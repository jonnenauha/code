/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include <cstdlib>

#include <sys/times.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/ref.hpp>

using namespace std;

struct event;
struct eventlog;
struct lock;
struct component;
struct component_transaction;
struct test;

struct component_transaction_exception {};
struct component_transaction_inconsistent_exception {};

typedef int                         field_t;
typedef unsigned int                bit_string_t;
typedef unsigned char               byte_t;

typedef field_t                     field_shared_t;
typedef bit_string_t                field_dirty_t;
typedef boost::recursive_mutex      mutex_t;

typedef bit_string_t                field_name_t;
typedef map <field_name_t,field_t>  field_log_t;

typedef vector <component_transaction *> component_transaction_list_t;

const int NUM_THREADS = 3;
const int NUM_FIELDS = 3;
const int FIELD_X = 0;
const int FIELD_Y = 1;
const int FIELD_Z = 2;

bool inconsistent (false);
int transactions (0), aborts (0), inconsistencies (0), runs (0);

inline void set_bits (bit_string_t &b, bit_string_t c) { b |= c; }
inline void clear_bits (bit_string_t &b, bit_string_t c) { b &= ~c; }
inline void clear_bits (bit_string_t &b) { b ^= b; }
inline bit_string_t bits_from_field (field_name_t f) { return (1 << f); }

struct event
{
    int timestamp;
    int id;
    int value;
    string msg;

    event (int ts, int i, const string &m) 
        : timestamp (ts), id (i), value (-1), msg (m) {}

    event (int ts, int i, int v, const string &m) 
        : timestamp (ts), id (i), value (v), msg (m) {}

    event (const event &e)
        : timestamp (e.timestamp), id (e.id), value (e.value), msg (e.msg) {}

    bool operator< (const event &rhs) const { return (timestamp < rhs.timestamp); }
};
    
ostream &operator<< (ostream &out, const event &e)
{
    out << "(" << e.timestamp << ") [" << e.id << "] " << e.msg; 
    if (e.value >= 0) out << " : " << e.value; 
    return out;
}

struct eventlog
{
    vector <event> log;
    int threadid;

    timeval begin;
    timeval end;

    eventlog (int id) 
        : threadid (id)
    { 
        log.reserve (2048);
        gettimeofday (&begin, NULL);
    }

    eventlog (const eventlog &e)
        : log (e.log), threadid (e.threadid), begin (e.begin), end (e.end)
    {}

    void post (const string &msg) 
    { 
        gettimeofday (&end, NULL); 
        log.push_back (event ((end.tv_usec - begin.tv_usec), threadid, msg)); 
    }

    void post (int value, const string &msg) 
    { 
        gettimeofday (&end, NULL); 
        log.push_back (event ((end.tv_usec - begin.tv_usec), threadid, value, msg)); 
    }
};

struct lock
{
    mutex_t *mutex;

    lock (mutex_t &m) : mutex (&m) { mutex->lock(); }
    ~lock () { mutex->unlock(); }
};

struct component : boost::noncopyable
{
    // <shared-data>
    field_shared_t  fields [NUM_FIELDS];
    mutex_t         fields_lock;

    field_dirty_t   dirty;
    mutex_t         dirty_lock;

    component_transaction_list_t    active;
    mutex_t                         active_lock;
    // </shared-data>

    component () : dirty (0) { fill_n (fields, NUM_FIELDS, 0); }

    void set_field (field_name_t f, field_t v) { fields [f] = v; }
    field_t get_field (field_name_t f) const { return fields [f]; }

    void dirty_field (field_name_t f) { set_bits (dirty, bits_from_field (f)); }
    void clean_field (field_name_t f) { clear_bits (dirty, bits_from_field (f)); }
    void clean_fields (bit_string_t b) { clear_bits (dirty, b); }

    void add_transaction (component_transaction *t) { active.push_back (t); }
    void remove_transaction (component_transaction *t) { active.erase (find (active.begin(), active.end(), t)); }
};

struct component_transaction_view
{
    component       *comp;

    field_t         shadow_fields [NUM_FIELDS];
    field_dirty_t   touched;

    component_transaction_view (component *c)
        : comp (c), touched (0)
    {
        copy (c->fields, c->fields+NUM_FIELDS, shadow_fields);
    }

    field_t read (field_name_t f)
    {
        touched |= bits_from_field (f);
        return shadow_fields [f] = comp-> fields [f];
    }

    void write (field_name_t f, field_t v)
    {
        comp-> fields [f] = shadow_fields [f] = v;
        touched |= bits_from_field (f);
    }

    bool consistent (field_name_t f)
    {
        if ((touched & bits_from_field (f)) && (comp-> fields[f] != shadow_fields[f]))
            return false;
        else
            return true;
    }

    bool consistent ()
    {
        for (int i=0; i < NUM_FIELDS; ++i)
            if (!consistent (i))
                return false;
        return true;
    }
};

struct component_transaction : boost::noncopyable
{
    component                   *comp;
    component_transaction_view  view;

    eventlog        &el;

    bit_string_t    write_delta;
    bit_string_t    read_delta;
    bit_string_t    change_delta;
    field_log_t     log;
    
    bool write_inconsistency;
        
    component_transaction (component *c, eventlog &log) 
        : comp (c), view (c), el (log), 
        write_delta (0), read_delta (0), change_delta (0),
        write_inconsistency (false)
    {
        el.post ("locked for active add");
        lock l (comp-> active_lock);
        comp-> add_transaction (this);
    }

    ~component_transaction () 
    {
        {
            el.post ("locked for active remove");
            lock l (comp-> active_lock);
            comp-> remove_transaction (this);
        }

        if (write_delta) rollback(); // uncommitted writes
    }

    void log_write (field_name_t f)
    {
        el.post ("log write");
        set_bits (write_delta, bits_from_field (f));
        log.insert (make_pair (f, comp-> get_field (f))); // log clean copy
    }

    void log_read (field_name_t f)
    {
        el.post ("log read");
        set_bits (read_delta, bits_from_field (f));
    }

    void check (bit_string_t b = 0)
    {
        el.post ("check");
        el.post (comp->dirty, "dirty");
        el.post (write_delta, "write_delta");
        el.post (read_delta, "read_delta");
        el.post (change_delta, "change_delta");
        el.post (b, "check delta");

        bit_string_t foreign_writes (comp->dirty ^ write_delta);
        bit_string_t local_delta (read_delta | write_delta);

        if ((change_delta & local_delta) || (foreign_writes & read_delta) || (foreign_writes & b)) 
        {
            rollback();
            throw component_transaction_exception ();
        }
    }

    void get (field_name_t f, field_t &v)
    {
        el.post ("get");

        v = view.read (f);
        log_read (f);

        check();
    }

    void set (field_name_t f, field_t v)
    {
        el.post ("set");

        {
            el.post ("locked for set");
            lock l (comp->dirty_lock);

            check (bits_from_field (f));
            
            if (!view.consistent(f))
            {
                el.post ("INCONSISTENCY DETECTED AFTER WRITE CHECK");
                el.post (view.touched, "view touched");
                el.post (view.shadow_fields [FIELD_X], "shadow X");
                el.post (view.shadow_fields [FIELD_Y], "shadow Y");
                el.post (view.shadow_fields [FIELD_Z], "shadow Z");
                el.post (comp->dirty, "dirty");
                el.post (comp->fields [FIELD_X], "X");
                el.post (comp->fields [FIELD_Y], "Y");
                el.post (comp->fields [FIELD_Z], "Z");

                write_inconsistency = true;
            }
            
            log_write (f);
            view.write (f, v);

            comp-> dirty_field (f);

            el.post (comp-> dirty, "new dirty");
            el.post ("unlocked from set");
        }
    }

    void rollback ()
    {
        el.post ("rollback");

        {
            lock l1 (comp->dirty_lock); el.post ("locked for dirty rollback");
            lock l2 (comp->fields_lock); el.post ("locked for field rollback");

            field_log_t::iterator i (log.begin());
            for (; i != log.end(); ++i)
            {
                comp-> set_field (i->first, i->second);
                el.post (i->first, "rollback field");
                el.post (i->second, "rollback value");
            }

            notify_change ();
        
            comp-> clean_fields (write_delta);
            el.post ("unlocked from rollback");
        }
    }

    void commit ()
    {
        el.post ("commit");

        {
            lock l (comp->dirty_lock); el.post ("locked for dirty clean");

            check();

            if (!view.consistent() || write_inconsistency)
            {
                el.post ("INCONSISTENCY DETECTED AFTER COMMIT CHECK");
                el.post (view.touched, "view touched");
                el.post (view.shadow_fields [FIELD_X], "shadow X");
                el.post (view.shadow_fields [FIELD_Y], "shadow Y");
                el.post (view.shadow_fields [FIELD_Z], "shadow Z");
                el.post (comp->dirty, "dirty");
                el.post (comp->fields [FIELD_X], "X");
                el.post (comp->fields [FIELD_Y], "Y");
                el.post (comp->fields [FIELD_Z], "Z");

                inconsistent = true;
                throw component_transaction_inconsistent_exception ();
            }
            
            notify_change ();
        
            comp-> clean_fields (write_delta);
            el.post ("unlocked from commit");
        }

        clear_bits (write_delta);
    }

    void notify_change ()
    {
        lock l (comp->active_lock); el.post ("locked for change notify");

        el.post ("send change_deltas");
        component_transaction_list_t::iterator i (comp-> active.begin());
        for (; i != comp-> active.end(); ++i)
            (*i)-> change_delta |= write_delta; 
            
        el.post ("unlocked from change notify");
    }
};


struct test
{
    eventlog log;
    int id;

    int aborts;
    int transactions;
    int inconsistencies;

    test (int i) 
        : log (i), id (i), 
        aborts (0), transactions (0), 
        inconsistencies (0) {}

    void operator() (component *c)
    {
        for (;;)
        {
            if (inconsistent)
                break;

            if (transactions > 200)
                break;

            if (aborts > 10)
                break;

            log.post (c->fields[0], "FIELD_X");
            log.post (c->fields[1], "FIELD_Y");
            log.post (c->fields[2], "FIELD_Z");
            log.post (c->dirty, "dirty");

            int x, y, r;
            field_name_t read_field, readwrite_field;

            try
            {
                log.post ("===================");
                component_transaction t (c, log);
                log.post ("begin transaction");

                read_field = rand() % NUM_FIELDS; 
                readwrite_field = rand() % NUM_FIELDS; 
                r = rand() % 100;

                log.post (read_field, "read_field");
                log.post (readwrite_field, "readwrite_field");

                log.post ("read section");

                t.get (read_field, x);
                t.get (readwrite_field, y);

                log.post ("write section");  

                t.set (readwrite_field, r);

                log.post (readwrite_field, "readwrite_field");
                log.post (r, "r");

                log.post ("commit section");

                log.post ("begin transaction commit");
                t.commit ();
                log.post ("end transaction commit");

                ++ transactions;
            }

            catch (component_transaction_exception &e)
            {
                log.post ("ABORTED transaction");
                ++ aborts;
            }

            catch (component_transaction_inconsistent_exception &e)
            {
                log.post ("INCONSISTENT transaction");
                ++ inconsistencies;

                log.post (c->get_field (read_field), "read_field");
                log.post (c->get_field (readwrite_field), "readwrite_field");
                log.post (x, "x");
                log.post (y, "y");
                log.post (r, "r");
            }
        }
    }
};

void print ()
{
    cout << "runs: " << runs << endl;
    cout << "transactions: " << transactions << endl;
    cout << "aborts: " << aborts << endl;
    cout << "inconsistencies: " << inconsistencies << endl;
}

void siginthandle (int) 
{ 
    print (); 
    exit (0);
}

//=============================================================================
// Main entry point
int main (int argc, char** argv)
{   
	signal (SIGINT, siginthandle);

    component c;

    cout << "sizeof (field_t) : " << sizeof (field_t) << endl;
    cout << "sizeof (mutex_t) : " << sizeof (mutex_t) << endl;
    cout << "sizeof (field_log_t) : " << sizeof (field_log_t) << endl;
    cout << "sizeof (component) : " << sizeof (component) << endl;
    cout << "sizeof (component_transaction) : " << sizeof (component_transaction) << endl;
    cout << "sizeof (component_transaction_view) : " << sizeof (component_transaction_view) << endl;

    while (inconsistencies < 1)
    {
        inconsistent = false;

        vector <test> tests;
        vector <test>::iterator it;

        vector <boost::thread *> threads;
        vector <boost::thread *>::iterator ith;

        for (int i=0; i < NUM_THREADS; ++i)
            tests.push_back (test (i));

        for (it = tests.begin(); it != tests.end(); ++it)
            threads.push_back (new boost::thread (boost::ref(*it), &c));

        for (ith = threads.begin(); ith != threads.end(); ++ith)
            (*ith)-> join();

        for (ith = threads.begin(); ith != threads.end(); ++ith)
            delete *ith;

        if (inconsistent)
        {
            cout << "EXCEPTION THROWN" << endl;

            vector <event> log;
            for (it = tests.begin(); it < tests.end(); ++it)
                log.insert (log.begin(), it->log.log.begin(), it->log.log.end());
            sort (log.begin(), log.end());

            vector <event>::iterator i (log.begin());
            for (; i != log.end(); ++i)
                cout << *i << endl;

            ++ inconsistencies;
        }

        for (it = tests.begin(); it < tests.end(); ++it)
            transactions += it-> transactions;

        for (it = tests.begin(); it < tests.end(); ++it)
            aborts += it-> aborts;

        ++ runs;
    }

    print ();

    return 0;
}


