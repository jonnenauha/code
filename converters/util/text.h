/* util.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include "logstream/logstream.hpp"
#include "logstream/logiomanip.hpp"
#include "logstream/logmacro.hpp"
#include "logstream/logobject.hpp"

#include "types.h"

namespace ThreeDInc
{
    hash_t str_hash_fun (const std::string& s);
    std::iostream& reset (std::iostream& io);
    std::istream& getline (std::istream& in, std::stringstream& out);
    std::istream& ignoreline (std::istream& in);
    std::istream& readbrackets (std::istream& in, std::stringstream& out);
    std::istream& ignorebrackets (std::istream& in);
    std::istream& stripcomments (std::istream& in, std::stringstream& out);
}

#endif //_UTIL_H_
