/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <memory>

namespace ThreeDInc
{
    typedef size_t index_t;

    template <typename T, size_t Size, typename MemoryAllocator = StdMemoryAllocator<T> >
	class basic_array
	{
	    //-------------------------------------------------------------------------
	    public:
		// Constructors
		basic_array ()
		    : array_ (NULL), size_ (0)
		{}

		// Copy constructor
		basic_array (const basic_array& copy)
		    : array_ (NULL), size_ (0)
		{ 
		}

		// Destructor
		virtual ~basic_array ()
		{
		    try { memalloc_.release_array (array_); }
		    catch (...) 
		    { 
			std::cerr << "basic_array: exception in destructor" 
			    << std::endl; 
		    }
		}

		// Virtual copy constructor
		virtual basic_array* clone () const { return new basic_array (*this); }

		// Assignment operator
		// will resize the dimenions to fit the data
		basic_array& operator= (const basic_array& rhs)
		{ 
		    if (this == &rhs)
			return *this;

		    // swap copy

		    return *this;
		}

		// subarray_t access operator
		const subarray_t& operator[] (index_t i) const 
		{ assert (i < size_); return array_[i]; }

		subarray_t& operator[] (index_t i) 
		{ assert (i < size_); return array_[i]; }

		// Resize each of the array array's dimension
		// variable arguments interface
		void resize (size_t size, ...)
		{ 
		    size_t sizelist [size];
		    va_list arguments;
		    va_start (arguments, size);

		    sizelist[0] = size;
		    for (index_t i=1; i < size; ++i)
			sizelist[i] = va_arg (arguments, size_t);
		    va_end (arguments);

		    resize (sizelist);
		}

		// Resize each of the array's dimensions
		// list ptr interface
		void resize (size_t *listiter)
		{
		    size_t size = *listiter++;
		    resize_dimension (size);

		    subarray_t *sub = begin();
		    for (index_t i=0; i < size; ++i, ++sub)
			sub->resize (listiter);
		}

		// Resize the number of dimensions
		void resize_dimension (size_t size) 
		{ memalloc_.resize_array (array_, size_, size); }

		// Insert new subarray_t
		void insert (const subarray_t& sub, index_t position)
		{
		    assert (position < size_);

		    resize_dimension (size_+1);

		    subarray_t *pos = array_+position;
		    subarray_t *last = array_+size_-1;

		    std::copy (sub.begin(), sub.end(), last->begin());
		    swap (*pos, *last);
		}

		// Swap two arrays
		void swap (basic_array& rhs)
		{
		    subarray_t* ta = array_; size_t ts = size_;
		    array_ = rhs.array_; size_ = rhs.size_;
		    rhs.array_ = ta; rhs.size_ = ts;
		}

		// Iterator interface
		size_t			dimension_size () const { return size_; }
		subarray_t*	 	begin () { return array_; }
		const subarray_t*	begin () const { return array_; }
		subarray_t* 		end () { return array_+size_; }
		const subarray_t*	end () const { return array_+size_; }

		
		//-------------------------------------------------------------------------
	    private:
		subarray_t*		array_;
		size_t			size_;
		SubArrayMemoryAllocator	memalloc_;
	};

    //-------------------------------------------------------------------------
    // Base class for the template recursion
    template <typename T, typename MemoryAllocator>
	class basic_array <T,1,MemoryAllocator>
	{
	    typedef T* subarray_t;

	    public:
		// Constructors
		basic_array ()
		    : array_ (NULL), size_ (0)
		{}

		explicit basic_array (size_t size)
		    : array_ (NULL), size_ (0)
		{
		    if (size) resize (size);
		}

		basic_array (const T* array, size_t size)
		    : array_ (NULL), size_ (0)
		{
		    if (size)
		    {
			resize (size);
			std::copy (array, array+size, array_);
		    }
		}
		
		basic_array (const std::auto_ptr<T> array, size_t size)
		    : array_ (array.release()), size_ (size)
		{}


		// Resize this on copy
		basic_array (const basic_array<T,1>& copy)
		    : array_ (NULL), size_ (0)
		{
		    const size_t size = copy.size();

		    if (size)
		    {
			resize (size);
			std::copy (copy.begin(), copy.end(), begin());
		    }
		}

		virtual ~basic_array ()
		{
		    try { memalloc_.release_array (array_); }
		    catch (...) 
		    { std::cerr << "basic_array: exception in destructor" << std::endl; }
		}

		virtual basic_array* clone () const { return new basic_array (*this); }

		// Resize
		void resize (size_t *iter) { resize (*iter); }
		void resize (size_t size) {  memalloc_.resize_array (array_, size_, size); }

		// Assignment operator
		basic_array& operator= (const basic_array& rhs)
		{ 
		    if (this == &rhs)
			return *this;

		    basic_array t (rhs);
		    swap (t);

		    return *this;
		}

		// Access operator
		const T operator[] (index_t i) const { assert (i < size_); return array_[i]; }
		T& operator[] (index_t i) { assert (i < size_); return array_[i]; }

		// Quick swapping method
		void swap (basic_array& rhs)
		{
		    T* ta = array_; size_t ts = size_;
		    array_ = rhs.array_; size_ = rhs.size_;
		    rhs.array_ = ta; rhs.size_ = ts;
		}

		// Iterator interface
		size_t		dimension_size () const { return size_; }
		size_t		size () const { return size_; }
		T* 		begin () { return array_; }
		const T*	begin () const { return array_; }
		T* 		end () { return array_+size_; }
		const T*	end () const { return array_+size_; }

		//-------------------------------------------------------------------------
	    private:
		T* 		array_;
		size_t		size_;
		
		MemoryAllocator	memalloc_;
	};

    //=============================================================================
    // calls fast swap method
    template <typename T, size_t size>
	void swap (basic_array<T,size>& lhs, basic_array<T,size>& rhs) { lhs.swap (rhs); }

}
#endif //__ARRAY_HPP__
