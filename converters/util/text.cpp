/* util.cpp -- util module
 *
 *			Ryan McDougall -- 20070101
 */

#include <iostream>
#include <sstream>

#include <util/text.h>
#include <vrml-constants.h>

namespace ThreeDInc
{
    hash_t str_hash_fun (const std::string& s)
    {
	hash_t hash (0);

	std::string::const_iterator i (s.begin());
	const std::string::const_iterator end (s.end());

	for (; i != end; ++i)
	    hash = 5 * hash + *i;

	return hash;
    }
    
    std::iostream& reset (std::iostream& io)
    {
	io.clear();
	io.seekp (0);
	io.seekg (0);
	return io;
    }

    std::istream& getline (std::istream& in, std::stringstream& out)
    {
	int charbuf;

	for (;;)
	{
	    charbuf = in.get();
	    if (in.eof()) break;

	    if (charbuf == VRMLConstants::newline)
		break;

	    out.put (charbuf);
	}

	return in;
    }

    std::istream& ignoreline (std::istream& in)
    {
	int charbuf;

	for (;;)
	{
	    charbuf = in.get();
	    if (in.eof()) break;

	    if (charbuf == VRMLConstants::newline)
	    {
		in.putback (charbuf);
		break;
	    }
	}

	return in;
    }

    std::istream& readbrackets (std::istream& in, std::stringstream& out)
    {
	using namespace VRMLConstants;
	logstream& log = get_logstream ("readbrackets");

	int charbuf, beginbracket, endbracket;

	charbuf = in.get();
	if (in.eof()) return in;

	// detect bracket type
	if (charbuf == bracket_left_curl)
	{
	    beginbracket = charbuf;
	    endbracket = bracket_right_curl;
	}

	else if (charbuf == bracket_left_sq)
	{
	    beginbracket = charbuf;
	    endbracket = bracket_right_sq;
	}

	else // no bracket found
	{
	    in.putback (charbuf);
	    return in;
	}

	int bracketcount (1); 
	for (;;)
	{
	    charbuf = in.get(); 
	    if (in.eof()) break;

	    if (charbuf == beginbracket)
		++ bracketcount;

	    else if (charbuf == endbracket)
		-- bracketcount;

	    if (bracketcount == 0)
		break;
	    
	    else if (bracketcount < 0)
		log_error (log, "bad bracket match");

	    else
		out.put (charbuf);
	}

	return in;
    }

    std::istream& ignorebrackets (std::istream& in)
    {
	using namespace VRMLConstants;
	logstream& log = get_logstream ("ignorebrackets");

	int charbuf, endbracket, beginbracket;

	charbuf = in.get();
	if (in.eof()) return in;

	// detect bracket type
	if (charbuf == bracket_left_curl)
	{
	    beginbracket = charbuf;
	    endbracket = bracket_right_curl;
	}

	else if (charbuf == bracket_left_sq)
	{
	    beginbracket = charbuf;
	    endbracket = bracket_right_sq;
	}

	else // no bracket found
	{
	    in.putback (charbuf);
	    return in;
	}

	int bracketcount (1); 
	for (;;)
	{
	    charbuf = in.get(); 
	    if (in.eof()) break;

	    if (charbuf == beginbracket)
		++ bracketcount;

	    else if (charbuf == endbracket)
		-- bracketcount;

	    if (bracketcount == 0)
		break;
	    
	    else if (bracketcount < 0)
		log_error (log, "bad bracket match");
	}

	return in;
    }

    std::istream& stripcomments (std::istream& in, std::stringstream& out)
    {
	int charbuf;

	for (;;)
	{
	    charbuf = in.get();
	    if (in.eof()) break;
	    
	    if (charbuf == VRMLConstants::comment)
		ignoreline (in);
	    else
		out.put (charbuf);
	}

	return in;
    }

}
