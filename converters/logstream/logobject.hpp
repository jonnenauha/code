/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _LOGOBJECT_HPP_
#define _LOGOBJECT_HPP_


//=============================================================================
// Interface to provide a logging member to inheriting classes

template <typename LoggableClass>
class Loggable
{
    public:
	Loggable () :
	    log_ (&get_logstream (typeid (LoggableClass).name())) 
	{}

	logstream& log() { return *log_; }

    private:
	logstream *log_;
};

#endif //_LOGOBJECT_HPP_
