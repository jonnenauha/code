/* main.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <QtCore/QObject>
#include <QtDBus/QtDBus>

using namespace std;

class Test : public QObject
{
    Q_OBJECT

    public Q_SLOTS:
        void start (const QString &name, const QString &oldname, const QString &newname)
        {
            entityi = new QDBusInterface ("org.realxtend.Local", "/foo", "org.realxtend.Test.Entity", QDBusConnection::sessionBus(), this);
            servicei = new QDBusInterface ("org.realxtend.Local", "/bar", "org.realxtend.Test.Service", QDBusConnection::sessionBus(), this);

            if (!entityi->isValid() || !servicei->isValid()) 
            { 
                cerr << qPrintable (QDBusConnection::sessionBus().lastError().message()) << endl;
                //QCoreApplication::instance()-> quit();
            }

            QVariant preply;
            QDBusReply<int> mreply;

            QTime t; t.start();

            mreply = entityi->call ("go_nuts");

            //for (int i=0; i < 90000; ++i)
            //{
            //    // Properties
            //    entityi->setProperty ("component", "wtf");
            //    preply = entityi->property ("component");

            //    if (entityi->lastError().isValid())
            //        cerr << "error: " << qPrintable (entityi->lastError().message()) << endl;

            //    if (!preply.isNull())
            //        cout << "component = " << qPrintable (preply.toString()) << endl;

            //    // Methods
            //    mreply = servicei->call ("lart", 3);
            //    mreply = servicei->call ("lart", 2);

            //    if (mreply.isValid())
            //        cout << "Reply was: " << mreply.value() << endl;
            //    else
            //        cerr << "no reply?!" << endl;
            //}

            int elapsed (t.elapsed());
            cout << "test (elapsed ms): " << elapsed << endl;
        }

    void component_listener (QString c)
    {
        //cout << "component changed: " << qPrintable (c) << endl;
    }

    private:
        QDBusInterface *entityi;
        QDBusInterface *servicei;
};

#endif //_MAIN_H_
