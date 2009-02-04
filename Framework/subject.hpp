/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _SUBJECT_HPP_
#define _SUBJECT_HPP_

namespace Framework
{
    //=============================================================================
    class Subject : 
        public boost::signals::trackable
    {
        //-------------------------------------------------------------------------
        public:
            virtual void Notify () { on_notify_ (this, change_); }
            virtual void Dispose () { on_dispose_ (this); }
            virtual void Change (string part) = 0;

            //-------------------------------------------------------------------------
        protected:
            boost::signal <void (Subject *, SubjectChange)> on_notify_;
            boost::signal <void (Subject *)> on_dispose_;
            SubjectChange change_;
    };
}
#endif //_SUBJECT_HPP_
