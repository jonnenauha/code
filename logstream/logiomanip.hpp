/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef _LOGIOMANIP_HPP_
#define _LOGIOMANIP_HPP_

#include <ctime>
#include <sys/time.h>
#include <unistd.h>

using std::cout;
using std::endl;

//=============================================================================
//

// Context altering IO manipulators
template <typename C, typename T>
    basic_logstream <C,T>& operator << 
(basic_logstream <C,T>& out, 
 basic_logstream <C,T>& (*manip) (basic_logstream <C,T>&)) 
{ 
    typedef std::basic_ostream <C,T> basic_ostream_type;
    
    basic_logstream <C,T> *p = out.parent();
    if (p) *p << manip;

    return manip (out);
}

template <typename C, typename T>
    basic_logstream <C,T>& operator << 
(basic_logstream <C,T>& out, 
 std::basic_ostream <C,T>& (*manip) (std::basic_ostream <C,T>&)) 
{ 
    logstream_forward_manip_ (out, manip);
    return out; 
}

template <typename C, typename T>
    basic_logstream <C,T>& operator << 
(basic_logstream <C,T>& out, 
 std::basic_ios <C,T>& (*manip) (std::basic_ios <C,T>&)) 
{ 
    logstream_forward_manip_ (out, manip);
    return out; 
}

template <typename C, typename T>
    basic_logstream <C,T>& operator << 
(basic_logstream <C,T>& out, 
 std::basic_streambuf <C,T>& (*manip) (std::basic_streambuf <C,T>&)) 
{ 
    logstream_forward_manip_ (out, manip);
    return out; 
}

//-----------------------------------------------------------------------------
// Misc IO manipulators
std::string timestamp (time_t t) 
{ 
    std::string s (ctime (&t));
    return s.replace (s.size()-1, 1, 1, '\0');
}

suseconds_t microtimestamp ()
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return tv.tv_usec;
}

template<typename C, typename T>
    std::basic_ostream <C,T>&
timestamp (std::basic_ostream <C,T>& out) 
{ return out << timestamp (time (NULL)); }

template<typename C, typename T>
    std::basic_ostream <C,T>&
microtimestamp (std::basic_ostream <C,T>& out) 
{ return out << microtimestamp(); }

template<typename C, typename T>
    std::basic_ostream <C,T>&
pid (std::basic_ostream <C,T>& out) 
{ return out << getpid(); }

template<typename C, typename T>
    std::basic_ostream <C,T>&
ppid (std::basic_ostream <C,T>& out) 
{ return out << getppid(); }

//-----------------------------------------------------------------------------
// IO manipulators for setting log levels
template<typename C, typename T>
    basic_logstream <C,T>&
none (basic_logstream <C,T>& out) 
{ return out.context (NONE); }

template<typename C, typename T>
    basic_logstream <C,T>&
debug (basic_logstream <C,T>& out) 
{ return out.context (DEBUG); }

template<typename C, typename T>
    basic_logstream <C,T>&
info (basic_logstream <C,T>& out) 
{ return out.context (INFO); }

template<typename C, typename T>
    basic_logstream <C,T>&
warn (basic_logstream <C,T>& out) 
{ return out.context (WARN); }

template<typename C, typename T>
    basic_logstream <C,T>&
error (basic_logstream <C,T>& out) 
{ return out.context (ERROR); }

template<typename C, typename T>
    basic_logstream <C,T>&
fatal (basic_logstream <C,T>& out) 
{ return out.context (FATAL); }

#endif //_LOGIOMANIP_HPP_
