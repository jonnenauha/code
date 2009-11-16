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

#include <QCoreApplication>
#include <QObject>
#include <QtScript>

using namespace std;

class ExistingClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString capabilities READ capabilities WRITE set_capabilities)

    public:
        ExistingClass () : value_ (0) {}

        QString capabilities () const { return caps_; }
        void set_capabilities (const QString &s) { caps_ = s; }

    public Q_SLOTS:
        int value () const { return value_; }
        void set_value (int v) { value_ = v; }

        void foo ()
        {
            cout << value_ << endl;
            Q_EMIT poked ();
        }

        int lart (int v)
        {
            value_ += v;
            Q_EMIT action (value_);
        }

        void on_poke ()
        {
            cout << "someone poked me" << endl;
        }

    Q_SIGNALS:
        void action (int f);
        void poked ();

    private:
        int value_;
        QString caps_;
};

#endif //_MAIN_H_
