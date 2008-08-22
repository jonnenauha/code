/* main.cpp -- main module
 *
 *			Ryan McDougall -- 20070101
 */

#include "logstream.hpp"

//=============================================================================
std::basic_streambuf <char,std::char_traits<char> > * 
extended_char_traits <char>::default_rdbuf = std::clog.rdbuf();

std::basic_streambuf <wchar_t,std::char_traits<wchar_t> > * 
extended_char_traits <wchar_t>::default_rdbuf = std::wclog.rdbuf();

const std::basic_string <char,std::char_traits<char> >
extended_char_traits <char>::root_name ("root");

const std::basic_string <wchar_t,std::char_traits<wchar_t> > 
extended_char_traits <wchar_t>::root_name (L"root");

logstream& get_logstream (const std::string& path)
{
    return get_logstream_factory <char,std::char_traits<char> >()
	.get_logstream (path);
}

wlogstream& get_logstream (const std::wstring& path)
{
    return get_logstream_factory <wchar_t,std::char_traits<wchar_t> >()
	.get_logstream (path);
}

