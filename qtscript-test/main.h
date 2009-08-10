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

    public:
        ExistingClass () : foo_ (0) {}

    public Q_SLOTS:
        void foo ()
        {
            cout << foo_ << endl;
        }

        int lart (int v)
        {
            foo_ += v;
        }

    Q_SIGNALS:
        void action ();

    private:
        int foo_;
};

#endif //_MAIN_H_
