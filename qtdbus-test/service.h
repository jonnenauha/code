/* service.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <QtCore/QObject>

class Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value)

    public:
        Service () : value_ (0) {}
    
        int value () const { return value_; }

    public Q_SLOTS:
        int lart (int v) { value_ += v; return value_; }

    private:
        int value_;
};

#endif //_SERVICE_H_
