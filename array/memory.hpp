/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <memory>

namespace ThreeDInc
{
    //=============================================================================
    // Memory allocation policy that uses standard C++ new and delete operators
    template <typename T>
	class StdMemoryAllocator
	{
	    public:
		template <class U> struct rebind 
		{ typedef StdMemoryAllocator<U> other; };

		static T* get_array (size_t size) { return new T [size]; }

		static void release_array (T* mem) { delete [] mem; }

		static void resize_array (T*& mem, size_t& size, size_t newsize)
		{
		    if (newsize > size)
		    {
			std::auto_ptr <T> handle (get_array (newsize));

			if (mem)
			{
			    T *newptr = handle.get();
			    T *memptr = mem;
			    while (memptr < mem+size)
				swap (*newptr++, *memptr++);
			}

			delete [] mem;
			mem = handle.release ();
		    }

		    size = newsize;
		}
	};
}
#endif //_MEMORY_HPP_
