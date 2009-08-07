/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"

const int NPACKETS (35000);
QTime glb_timer;
int glb_time;

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QCoreApplication app (argc, argv);
    Network net; NetworkListener listener;

    qRegisterMetaType <NetworkInEvent> ();
    qRegisterMetaType <NetworkOutEvent> ();

    QObject::connect 
        (&net, SIGNAL (received(NetworkInEvent)), 
         &listener, SLOT (received(NetworkInEvent)));

    QObject::connect 
        (&net, SIGNAL (sent(NetworkOutEvent)), 
         &listener, SLOT (sent(NetworkOutEvent)));

    net.QThread::start();
    listener.QThread::start();

    return app.exec();
}
