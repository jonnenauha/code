/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef _LOGIOMANIP_HPP_
#define _LOGIOMANIP_HPP_

#include <ctime>
#include <sys/time.h>
#include <unistd.h>

//=============================================================================
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

inline std::string timestamp (time_t t) 
{ 
    std::string s (ctime (&t));
    return s.replace (s.size()-1, 1, 1, '\0');
}

inline suseconds_t microtimestamp ()
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return tv.tv_usec;
}

// timestamp
template<typename C, typename T>
std::basic_ostream <C,T>&
timestamp (std::basic_ostream <C,T>& out) 
{ return out << timestamp (time (NULL)); }

// microtimestamp
template<typename C, typename T>
std::basic_ostream <C,T>&
microtimestamp (std::basic_ostream <C,T>& out) 
{ return out << microtimestamp(); }

// process id
template<typename C, typename T>
std::basic_ostream <C,T>&
pid (std::basic_ostream <C,T>& out) 
{ return out << getpid(); }

// parent process id
template<typename C, typename T>
std::basic_ostream <C,T>&
ppid (std::basic_ostream <C,T>& out) 
{ return out << getppid(); }

//-----------------------------------------------------------------------------
// IO manipulators for setting log levels

// pop the message context level
template<typename C, typename T>
basic_logstream <C,T>&
pop (basic_logstream <C,T>& out) 
{ return out.pop_context(); }

// push a NONE message context level
template<typename C, typename T>
basic_logstream <C,T>&
none (basic_logstream <C,T>& out) 
{ return out.push_context (NONE); }

// push a DEBUG message context level
template<typename C, typename T>
basic_logstream <C,T>&
debug (basic_logstream <C,T>& out) 
{ return out.push_context (DEBUG); }

// push a INFO message context level
template<typename C, typename T>
basic_logstream <C,T>&
info (basic_logstream <C,T>& out) 
{ return out.push_context (INFO); }

// push a WARN message context level
template<typename C, typename T>
basic_logstream <C,T>&
warn (basic_logstream <C,T>& out) 
{ return out.push_context (WARN); }

// push a ERROR message context level
template<typename C, typename T>
basic_logstream <C,T>&
error (basic_logstream <C,T>& out) 
{ return out.push_context (ERROR); }

// push a FATAL message context level
template<typename C, typename T>
basic_logstream <C,T>&
fatal (basic_logstream <C,T>& out) 
{ return out.push_context (FATAL); }

#endif //_LOGIOMANIP_HPP_
