/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <valarray>
#include "memory.hpp"
#include "liniterator.hpp"

namespace ThreeDInc
{
    //=============================================================================
    template <typename Matrix>
	struct matrix_traits
	{
	    typedef typename Matrix::value_type 	value_type;
	    typedef typename Matrix::difference_type	difference_type;
	    typedef typename Matrix::size_type 		size_type;
	    typedef typename Matrix::pointer 		pointer;
	    typedef typename Matrix::reference 		reference;
	    
	    typedef typename Matrix::const_value_type 	const_value_type;
	    typedef typename Matrix::const_pointer 	const_pointer;
	    typedef typename Matrix::const_reference 	const_reference;

	    static const size_type item_offset = Matrix::item_offset;
	    static const size_type line_offset = Matrix::line_offset;
	    static const size_type num_rows = Matrix::num_rows;
	    static const size_type num_cols = Matrix::num_cols;
	    static const size_type size = Matrix::size;
	};

    //=============================================================================
    template <typename T>
	struct matrix_traits <T[4]>
	{
	    typedef T 		value_type;
	    typedef ptrdiff_t	difference_type;
	    typedef size_t 	size_type;
	    typedef T*		pointer;
	    typedef T& 	reference;

	    typedef const T 	const_value_type;
	    typedef const T* 	const_pointer;
	    typedef const T& 	const_reference;

	    static const size_type item_offset = 1;
	    static const size_type line_offset = 2;
	    static const size_type num_rows = 2;
	    static const size_type num_cols = 2;
	    static const size_type size = 4;
	};

    //=============================================================================
    template <typename T>
	struct matrix_traits <T[9]>
	{
	    typedef T 		value_type;
	    typedef ptrdiff_t	difference_type;
	    typedef size_t 	size_type;
	    typedef T*		pointer;
	    typedef T& 	reference;

	    typedef const T 	const_value_type;
	    typedef const T* 	const_pointer;
	    typedef const T& 	const_reference;

	    static const size_type item_offset = 1;
	    static const size_type line_offset = 3;
	    static const size_type num_rows = 3;
	    static const size_type num_cols = 3;
	    static const size_type size = 9;
	};

    //=============================================================================
    template <typename T>
	struct matrix_traits <T[16]>
	{
	    typedef T 		value_type;
	    typedef ptrdiff_t	difference_type;
	    typedef size_t 	size_type;
	    typedef T*		pointer;
	    typedef T& 	reference;

	    typedef const T 	const_value_type;
	    typedef const T* 	const_pointer;
	    typedef const T& 	const_reference;

	    static const size_type item_offset = 1;
	    static const size_type line_offset = 4;
	    static const size_type num_rows = 4;
	    static const size_type num_cols = 4;
	    static const size_type size = 16;
	};

    //=============================================================================
    //
    template <typename T, size_t NumRows, size_t NumCols>
	class matrix_view 
	{
	    //-------------------------------------------------------------------------
	    public:
		typedef T 		value_type;
		typedef const T 	const_value_type;
		typedef ptrdiff_t 	difference_type;
		typedef size_t 		size_type;

		typedef T* 		pointer;
		typedef const T*	const_pointer;
		typedef T&		reference;
		typedef const T&	const_reference;

		static const size_type item_offset = 1;
		static const size_type line_offset = NumCols;
		static const size_type num_rows = NumRows;
		static const size_type num_cols = NumCols;
		static const size_type size = NumRows * NumCols;

		// Constructors and basic operators
		matrix_view (pointer p, difference_type i=0, difference_type j=0) 
		    : ptr_ (p), i_ (i), j_ (j) {}

		matrix_view (const matrix_view& rhs) 
		    : ptr_ (rhs.ptr_), i_ (rhs.i_), j_ (rhs.j_) {}
		
		// Access methods
		pointer operator [] (difference_type n) 
		{ return ptr_ + (i_+ n) * line_offset + j_; }

		const_pointer operator [] (difference_type n) const 
		{ return ptr_ + (i_+ n) * line_offset + j_; }

	    private:
		matrix_view ();

	    private:
		T* ptr_; 
		difference_type i_; 
		difference_type j_;
	};

    template <typename Matrix>
	matrix_view 
	<typename matrix_traits<Matrix>::value_type,
	matrix_traits<Matrix>::num_rows,
	matrix_traits<Matrix>::num_cols>

	make_matrix_view 
	(typename matrix_traits<Matrix>::pointer p, 
	 typename matrix_traits<Matrix>::difference_type i=0,
	 typename matrix_traits<Matrix>::difference_type j=0)
	{ 
	    return matrix_view 
		<typename matrix_traits<Matrix>::value_type,
		matrix_traits<Matrix>::num_rows,
		matrix_traits<Matrix>::num_cols> (p, i, j);
	}

    //=============================================================================
    //
    template <typename T, size_t NumRows, size_t NumCols>
	class matrix_transpose_view 
	{
	    //-------------------------------------------------------------------------
	    public:
		typedef T 		value_type;
		typedef const T 	const_value_type;
		typedef ptrdiff_t 	difference_type;
		typedef size_t 		size_type;

		typedef T* 		pointer;
		typedef const T*	const_pointer;
		typedef T&		reference;
		typedef const T&	const_reference;

		static const size_type item_offset = 1;
		static const size_type line_offset = NumCols;
		static const size_type num_rows = NumRows;
		static const size_type num_cols = NumCols;
		static const size_type size = NumRows * NumCols;

	    private:
		matrix_transpose_view ();

		struct transpose_expression_
		{
			transpose_expression_ (pointer p, difference_type index) 
			    : ptr (p), i (index) {}

			inline reference operator [] (difference_type j) 
			{ return *(ptr + j * line_offset + i); }

			inline const_reference operator [] (difference_type j) const 
			{ return *(ptr + j * line_offset + i); }

			pointer ptr;
			difference_type i;
		};

	    public:
		// Constructors and basic operators
		matrix_transpose_view (pointer p) : ptr_ (p) {}
		matrix_transpose_view (const matrix_transpose_view& rhs) : ptr_ (rhs.ptr_) {}
		
		// Access methods
		transpose_expression_ operator [] (difference_type i) { return transpose_expression_ (ptr_, i); }
		const transpose_expression_ operator [] (difference_type i) const { return transpose_expression_ (ptr_, i); }

	    private:
		T* ptr_; 
	};

    template <typename Matrix>
	matrix_transpose_view 
	<typename matrix_traits<Matrix>::value_type,
	matrix_traits<Matrix>::num_rows,
	matrix_traits<Matrix>::num_cols>

	make_matrix_transpose_view (typename matrix_traits<Matrix>::pointer p) 
	{ 
	    return matrix_transpose_view 
		<typename matrix_traits<Matrix>::value_type,
		matrix_traits<Matrix>::num_rows,
		matrix_traits<Matrix>::num_cols> (p);
	}

}

#endif //_MATRIX_HPP_
