/* entity.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <iostream>
#include <QtCore/QObject>

class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString component READ component WRITE set_component)

    public:
        Entity () {}

        QString component () const 
        { 
            return component_; 
        }

        void set_component (QString v) 
        { 
            component_ = v;
            Q_EMIT component_changed (v);
        }

    public Q_SLOTS:
        void go_nuts ()
        {
            std::cout << "wtf" << std::endl;
            for (int i=0; i < 15000; ++i)
                Q_EMIT component_changed ("hello");
        }

    Q_SIGNALS: 
        void component_changed (const QString &c);

    private:
        QString component_;
};

#endif //_ENTITY_H_
