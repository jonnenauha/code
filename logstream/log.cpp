/* main.cpp -- main module

 *			Ryan McDougall -- 20060101
 */

#include <iostream>
#include <sstream>
#include <cassert>
#include "logstream.hpp"
#include "logmacro.hpp"
#include "logiomanip.hpp"

using namespace std;

//=============================================================================
// Main entry point
int main (int argc, char** argv)
{

    stringbuf buf (ios_base::out);
    logstream_factory fact;
    
    assert (fact.set_buffer ("com.foo.tang.clan", &buf));
    assert (fact.set_buffer ("com.foo", cout.rdbuf()));
    
    logstream *dummylog = fact.get_logger ("com.foo.tang.clan");
    logstream *log = fact.get_logger ("com.foo.tang.clan");
    logstream *parent = fact.get_logger ("com.foo");

    assert (log == dummylog);
    
    fact.set_priority ("com.foo.tang.clan", ERROR);
    fact.set_priority ("com.foo", FATAL);

    *dummylog << "good morning";
    *dummylog << endl;
    
    log_fatal (log, pid << " " << microtimestamp << "w00t");
    
    (*log) << warn << "woot" << endl;
    (*log) << error << "poot" << endl;
    (*dummylog) << warn << "foo" << endl;
    (*parent) << fatal << "just me" << endl;

    cout << "log: " << buf.str();
    
    /*
    size_t num = 5;
    logstream parent ("root", NONE, cerr);
    logstream log ("child", FATAL, cerr, &parent);
    logstream log1 ("child1", WARN, &buf, &parent);

    log << none
	<< "bar"
	<< debug
	<< "foo"
	<< info
	<< num
	<< warn
	<< "baz"
	<< error
	<< "woot"
	<< fatal
	<< "poot";
    log << endl;

    log1 << none << "noon" << fatal << "fatal" << endl;
    cout << "str: " << buf.str() << endl;
*/
    return 0;
}
