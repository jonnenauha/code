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
                QCoreApplication::instance()-> quit();
            }

            // Properties
            entityi->setProperty ("component", "wtf");
            if (entityi->lastError().isValid())
                cerr << "error: " << qPrintable (entityi->lastError().message()) << endl;

            QVariant preply = entityi->property ("component");
            if (entityi->lastError().isValid())
                cerr << "error: " << qPrintable (entityi->lastError().message()) << endl;

            if (!preply.isNull())
                cout << "component = " << qPrintable (preply.toString()) << endl;

            // Methods
            QDBusReply<int> mreply = servicei->call ("lart", 3);
            if (mreply.isValid())
                cout << "Reply was: " << mreply.value() << endl;
            else
                cerr << "no reply?!" << endl;

            mreply = servicei->call ("lart", 2);
            if (mreply.isValid())
                cout << "Reply was: " << mreply.value() << endl;
            else
                cerr << "no reply?!" << endl;
        }

    private:
        QDBusInterface *entityi;
        QDBusInterface *servicei;
};

#endif //_MAIN_H_
