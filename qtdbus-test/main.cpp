/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "entity.h"
#include "entity_adaptor.h"
#include "service.h"
#include "service_adaptor.h"

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QCoreApplication app (argc, argv);

    Entity e;
    Service s;
    Test t;

    cout << "sizeof (QObject): " << sizeof (QObject) << endl;

    if (!QDBusConnection::sessionBus().isConnected()) 
    {
        cerr << "Cannot connect to the D-Bus session bus.\n"
            "To start it, run:\n"
            "\teval `dbus-launch --auto-syntax`\n" << endl;
        return 1;
    }

    EntityAdaptor *entadapter = new EntityAdaptor (&e);
    ServiceAdaptor *srvadapter = new ServiceAdaptor (&s);

    QDBusConnection::sessionBus().registerObject ("/foo", &e);
    QDBusConnection::sessionBus().registerObject ("/bar", &s);

    if (!QDBusConnection::sessionBus().registerService ("org.realxtend.Local")) 
    {
        cerr << qPrintable (QDBusConnection::sessionBus().lastError().message()) << endl;
        exit(1);
    }
    
    // Subscribe to Signals
    t.connect 
        (entadapter, 
         SIGNAL (component_changed(QString)), 
         SLOT (component_listener(QString)));

    // Start when a new service is added (us)
    t.connect 
        (QDBusConnection::sessionBus().interface(), 
         SIGNAL (serviceOwnerChanged(QString,QString,QString)), 
         SLOT (start(QString,QString,QString)));

    app.exec ();
    return 0;
}
