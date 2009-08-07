/* main.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QSharedData>
#include <QtCore/QMetaType>

using namespace std;

// use boost singletons
template <typename T>
class Singleton
{
    public:
        static T *instance ()
        {
            static T instance;
            return &instance;
        }
};

// pretend this is the naali programming framework
class Framework : public QObject, public Singleton <Framework>
{
    Q_OBJECT

    public:
    private:
};

// shared data is reference counted 
struct NetworkInData : public QSharedData
{
        NetworkInData (std::string &d) : data (d) {}
        std::string data;
};

// shared data is reference counted 
struct NetworkOutData : public QSharedData
{
        NetworkOutData (std::string d) : data (d) {}
        std::string data;
};

// event points to the packet data
// shared data pointer deletes data when refcount == 0
class NetworkInEvent
{
    public:
        NetworkInEvent () {}
        NetworkInEvent (const NetworkInEvent &c) : packet (c.packet) {}
        NetworkInEvent (NetworkInData *d) : packet (d) {}
        QExplicitlySharedDataPointer <NetworkInData> packet;
};

Q_DECLARE_METATYPE (NetworkInEvent); // required for signal/slot

// event points to the packet data
// shared data pointer deletes data when refcount == 0
class NetworkOutEvent
{
    public:
        NetworkOutEvent () {}
        NetworkOutEvent (const NetworkOutEvent &c) : packet (c.packet) {}
        NetworkOutEvent (NetworkOutData *d) : packet (d) {}
        QExplicitlySharedDataPointer <NetworkOutData> packet;
};

Q_DECLARE_METATYPE (NetworkOutEvent); // required for signal/slot

// wraps packet data into events, which handle refcounting
class Network : public QThread
{
    Q_OBJECT

    public:
        Network () 
            : n_ (10)
        {
            std::stringstream ss;
            for (int i=0; i < n_; ++i)
            {
                ss << i;
                inqueue_.push_back ("in " + ss.str());
                outqueue_.push_back ("out " + ss.str());
                ss.seekp (0);
            }
        }

    Q_SIGNALS:
        void received (NetworkInEvent);
        void sent (NetworkOutEvent);

    protected:
        void run ()
        {
            for (int i=0; i < n_; ++i)
            {
                cout << "sending packets!" << endl;

                NetworkInEvent in (new NetworkInData (inqueue_[i]));
                NetworkOutEvent out (new NetworkOutData (outqueue_[i]));

                Q_EMIT received (in);
                Q_EMIT sent (out);
            }
        }

    private:
        int n_;
        std::vector <std::string> inqueue_;
        std::vector <std::string> outqueue_;
};

// When a module chooses to accept an event, it can "detach" and receive its 
// own copy of the the shared data, or operate directly on it. There is no way 
// to deny other listeners a copy of the shared data.
class NetworkListener : public QThread
{
    Q_OBJECT

    public:
        NetworkListener () { }

    public Q_SLOTS:

        void received (NetworkInEvent event)
        {
            cout << "received: " << event.packet->data << endl;
        }

        void sent (NetworkOutEvent event)
        {
            cout << "sent: " << event.packet->data << endl;
        }

    protected:
        void run () { while (true) QThread::usleep (1000); }

    private:
};

#endif //_MAIN_H_
