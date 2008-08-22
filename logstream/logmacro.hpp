/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef _LOGMACRO_HPP_
#define _LOGMACRO_HPP_

#include <ostream>
#include <sstream>

using std::cout;
using std::endl;

//-----------------------------------------------------------------------------
// Convenience class for pretty printing log entries
template <typename CharType, typename TraitType=std::char_traits<CharType> >
class basic_log_entry : public std::basic_ostringstream <CharType,TraitType>
{
    public:
	typedef std::basic_string <CharType,TraitType> string_type;

	basic_log_entry (const string_type& f, const string_type& n, size_t l)
	    : filename_ (f), function_ (n), linenum_ (l)
	{}
	
	basic_log_entry (const CharType* f, const CharType* n, size_t l)
	    : filename_ (f), function_ (n), linenum_ (l)
	{}

	basic_log_entry (const basic_log_entry& copy)
	    : filename_ (copy.filename_), function_ (copy.function_), linenum_ (copy.linenum_)
	{}
	
	string_type filename () const { return filename_; }
	string_type function () const { return function_; }
	size_t linenum () const { return linenum_; }

    private:
	string_type filename_;
	string_type function_;
	size_t linenum_;

    private:
	basic_log_entry ();
};

typedef basic_log_entry<char> log_entry;
typedef basic_log_entry<wchar_t> wlog_entry;


//-----------------------------------------------------------------------------
// Forward all input to ostream if loggin at the given level
// and forward to parent
template <typename C, typename T>
    basic_logstream <C,T>& operator << 
(basic_logstream <C,T>& out, const basic_log_entry <C,T>& entry)
{
    typedef std::basic_ostream <C,T> basic_ostream_type;
    typedef typename basic_ostream_type::sentry sentry;
    
    basic_logstream <C,T> *p = out.parent();
    if (p) *p << entry;

    if ((out.context() >= out.priority()) && sentry (out))
	reinterpret_cast <basic_ostream_type&> 
	    (out) << '[' << out.name() << ' ' << out.context() << ']' 
	    << '(' << entry.filename() << ' ' << entry.function() 
	    << ':' << entry.linenum() << ')' << entry.str();

    return out;
}

//-----------------------------------------------------------------------------
// Helpful macros
#define log_debug(logger,msg) logger->context(DEBUG)<<log_entry(__FILE__,__FUNCTION__,__LINE__)<<msg<<std::endl
#define log_info(logger,msg) logger->context(INFO)<<log_entry(__FILE__,__FUNCTION__,__LINE__)<<msg<<std::endl
#define log_warn(logger,msg) logger->context(WARN)<<log_entry(__FILE__,__FUNCTION__,__LINE__)<<msg<<std::endl
#define log_error(logger,msg) logger->context(ERROR)<<log_entry(__FILE__,__FUNCTION__,__LINE__)<<msg<<std::endl
#define log_fatal(logger,msg) logger->context(FATAL)<<log_entry(__FILE__,__FUNCTION__,__LINE__)<<msg<<std::endl


#endif //_LOGMACRO_HPP_
