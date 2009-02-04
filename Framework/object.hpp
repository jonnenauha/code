/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

namespace Framework
{
    //=============================================================================
    class Object
    {
        //-------------------------------------------------------------------------
        public:
            // Constructors
            Object ()
            {}

            Object (const Object& rhs)
            {}

            // Virtual copy constructor
            virtual Object* clone () const
            {
                return new Object (*this); 
            }

            // Destructor
            virtual ~Object ()
            {
                try
                {}
                catch (...)
                {}
            }

            // Assignment operator
            Object& operator= (const Object& rhs)
            { 
                if (this == &rhs)
                    return *this;

                Object t (rhs);
                std::swap (*this, t);

                return *this;
            }

            object_type_id_t Id () { return object_id_; }

        protected:
            object_type_id_t object_id_;

        private:
    };
}

#endif //_OBJECT_HPP_
