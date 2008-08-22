/* main.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <cmath>
#include <functional>
#include <stdexcept>

namespace ThreeDInc
{
    template <typename T>
    struct geometric_sequence 
    : public std::binary_function <T,T,T>
    {
	T next;
	geometric_sequence (T init = T(0)) throw() 
	    : next (init) {}
	
	T operator () (T lhs, T rhs) const
	{ 
	    geometric_sequence *thisptr 
		= const_cast <geometric_sequence*> (this);

	    T current (thisptr->next); 
	    thisptr->next += lhs + rhs; 
	    return current;
	}
    };
}

#endif //_UTIL_HPP_
