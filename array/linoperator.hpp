/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _LINOPERATOR_HPP_
#define _LINOPERATOR_HPP_

#include <functional>
#include <numeric>
#include <cmath>

#include "liniterator.hpp"
#include "matrix.hpp"

namespace ThreeDInc
{
    //=============================================================================
    // Test int is even
    bool is_even (int n)
    { 
	if (n < 0) n = -n;
	return (n & 1)? false : true;
    }

    //=============================================================================
    // Test float for zero
    bool is_zero (double n) 
    { 
	static const double eps (1e-10);
	return (std::fabs (n) < eps); 
    }

    //=============================================================================
    // Sign of a float
    double sign (double a, double b)
    {
	double t = fabs (a);
	return (b >= 0.0)? t : -t;
    }

    //=============================================================================
    // Return the sign
    //template <typename T> inline
    //	T sign (const T& n)
    //	{ return (n < 0)? T(-1.0) : T(1.0); }

    //=============================================================================
    // Square a number
    template <typename T> inline 
	T sq (const T& n) { return n * n; }

    //=============================================================================
    // Return power by repeated squares
    template <typename T> T power (const T x, unsigned int n)
    {
	T result = T (1.0);
	T base = x;

	while (n)
	    if (is_even (n))
	    {
		base *= base;
		n /= 2;
	    }
	    else
	    {
		result *= base;
		--n;
	    }

	return result;
    }

    //=============================================================================
    // Computes pythagoras
    template <typename T> T pythagoras (T a, T b)
    {
	T absa (fabs (a)), 
	  absb (fabs (b));

	if (absa > absb) 
	    return absa * sqrt (1.0 + sq (absb/absa));
	else 
	    return ((absb == 0.0)? 
		    0.0 : absb * sqrt (1.0 + sq (absa/absb)));
    }


    //-----------------------------------------------------------------------------

    //=============================================================================
    // Transpose a matrix
    template <typename InputMatrix, typename OutputMatrix> 
	OutputMatrix transpose (InputMatrix A, OutputMatrix B)
	{
	    typename matrix_traits<InputMatrix>::size_type 
		m (matrix_traits<InputMatrix>::num_rows);

	    typename matrix_traits<InputMatrix>::size_type 
		n (matrix_traits<InputMatrix>::num_cols);

	    for (size_t i=0; i < m; ++i)
		for (size_t j=0; j < n; ++j)
		    B[j][i] = A[i][j];

	    return B;
	}

    //=============================================================================
    // Vector and Matrix Addition
    template <typename InputLinearIterator, typename OutputLinearIterator>
	OutputLinearIterator add 
	(InputLinearIterator lhs, InputLinearIterator end, 
	 InputLinearIterator rhs, OutputLinearIterator res)
	{
	    typedef typename linear_iterator_traits<InputLinearIterator>::value_type T;

	    std::transform (lhs, end, rhs, res, std::plus <T>());
	    return res;
	}

    //=============================================================================
    // Vector and Matrix Subtraction
    template <typename InputLinearIterator, typename OutputLinearIterator>
	OutputLinearIterator sub 
	(InputLinearIterator lhs, InputLinearIterator end, 
	 InputLinearIterator rhs, OutputLinearIterator res)
	{
	    typedef typename linear_iterator_traits<InputLinearIterator>::value_type T;

	    std::transform (lhs, end, rhs, res, std::minus <T>());
	    return res;
	}

    //=============================================================================
    // Vector and Matrix - Scalar Multiplication
    template <typename InputLinearIterator, typename OutputLinearIterator, typename T>
	OutputLinearIterator mul 
	(InputLinearIterator lhs, InputLinearIterator end, 
	 OutputLinearIterator res, T scale)
	{
	    std::transform (lhs, end, res, 
		    std::bind1st (std::multiplies <T>(), scale));

	    return res;
	}

    //=============================================================================
    // Vector and Matrix - Scalar Division
    template <typename InputLinearIterator, typename OutputLinearIterator, typename T>
	OutputLinearIterator div 
	(InputLinearIterator lhs, InputLinearIterator end, 
	 OutputLinearIterator res, T scale)
	{
	    std::transform (lhs, end, res, 
		    std::bind1st (std::divides <T>(), scale));

	    return res;
	}

    //=============================================================================
    // Vector dot product
    template <typename InputLinearIterator, typename T>
	T dot
	(InputLinearIterator lhs, InputLinearIterator end, 
	 InputLinearIterator rhs, T init)
	{ return std::inner_product (lhs, end, rhs, init); }

    //=============================================================================
    // Vector cross product
    template <typename InputLinearIterator, typename OutputLinearIterator>
	OutputLinearIterator cross
	(InputLinearIterator lhs, 
	 InputLinearIterator rhs, 
	 OutputLinearIterator res)
	{
	    *res++ = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	    *res++ = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	    *res = lhs[0] * rhs[1] - lhs[1] * rhs[0];

	    return res;
	}

    //=============================================================================
    // Vector norm
    template <typename LinearIterator, typename T>
	T norm
	(LinearIterator begin, LinearIterator end) 
	{ return sqrt (std::inner_product (begin, end, begin, T(0))); }

    //=============================================================================
    // Vector normalization
    template <typename LinearIterator>
	void normalize
	(LinearIterator begin, LinearIterator end) 
	{ 
	    typedef typename linear_iterator_traits<LinearIterator>::value_type T;

	    T n (norm (begin, end));

	    if (is_zero (n))
		throw std::runtime_error ("divide by zero");

	    n = T(1.0) / n;
	    mul (begin, end, begin, n);
	}

    //=============================================================================
    // Cartesian Product
    template <typename InputLinearIterator, typename OutputLinearIterator>
	OutputLinearIterator cartesian 
	(InputLinearIterator lhs, InputLinearIterator lend,
	 InputLinearIterator rhs, InputLinearIterator rend,
	 OutputLinearIterator res)
	{
	    InputLinearIterator rptr (rhs);
	    for (; lhs != lend; ++lhs, rptr = rhs)
		for (; rptr != rend; ++rptr, ++res)
		    *res = *lhs * *rptr;

	    return res;
	}

    //=============================================================================
    // Matrix - Matrix Product
    template <typename InputMatrixIterator1, typename InputMatrixIterator2, 
	     typename OutputLinearIterator>
	OutputLinearIterator mul_rowmajor
	(InputMatrixIterator1 lhs, InputMatrixIterator1 lhsend,
	 InputMatrixIterator2 rhs, InputMatrixIterator2 rhsend,
	 OutputLinearIterator res)
	{
	    typename matrix_iterator_traits<InputMatrixIterator1>::value_type 
		init (0);
	    
	    typename matrix_iterator_traits<InputMatrixIterator1>::size_type 
		lhsoff (matrix_iterator_traits<InputMatrixIterator1>::line_offset);

	    typename matrix_iterator_traits<InputMatrixIterator2>::size_type 
		rhsoff (matrix_iterator_traits<InputMatrixIterator2>::line_offset);

	    InputMatrixIterator1 row (lhs);
	    InputMatrixIterator2 col (rhs);

	    for (; row != lhsend; row += lhsoff, col = rhs)
		for (; col != rhsend; col += rhsoff, ++res)
		    *res = std::inner_product (row, row + lhsoff, col, init);

	    return res;
	}

    //=============================================================================
    // Matrix - Matrix Product
    template <typename InputMatrixIterator1, typename InputMatrixIterator2, 
	     typename OutputLinearIterator>
	OutputLinearIterator mul 
	(InputMatrixIterator1 lhs, InputMatrixIterator1 lhsend,
	 InputMatrixIterator2 rhs, InputMatrixIterator2 rhsend,
	 OutputLinearIterator res)
	{
	    typename matrix_iterator_traits<InputMatrixIterator1>::base_value_type 
		init (0);
	    
	    typename matrix_iterator_traits<InputMatrixIterator1>::size_type 
		ncol (matrix_iterator_traits<InputMatrixIterator1>::line_offset);

	    InputMatrixIterator1 row (lhs);
	    InputMatrixIterator2 col (rhs);

	    for (; row != lhsend; ++row, col = rhs)
		for (; col != rhsend; ++col, ++res)
		    *res = std::inner_product (*row, *row + ncol, *col, init);

	    return res;
	}

    //=============================================================================
    // Find the determinant
    template <typename InputMatrixIterator, typename T>
	T det (InputMatrixIterator mat)
       {
	   T res (0);

	   switch (matrix_iterator_traits<InputMatrixIterator>::size)
	   {
	       case 4:
		   res = det4 (mat);
		   break;

	       case 9:
		   res = det9 (mat);
		   break;

	       case 16:
		   res = det16 (mat);
		   break;

	       default:
		   throw std::logic_error ("generic det not implemented");
	   };

	   return res;
       }	   
    
    /*
    template <typename InputMatrixIterator, typename T>
	T det4 (InputMatrixIterator mat)
       {
	   typedef typename matrix_iterator_traits<InputMatrixIterator>::row_iterator
	       row_iterator;

	   row_iterator top (*mat++);
	   row_iterator bottom (*mat);

	   return top[0] * bottom[1] - top[1] * bottom[0];
       }	   
    
    template <typename InputMatrixIterator, typename T>
	T det9 (InputMatrixIterator mat)
       {
	   typedef typename matrix_iterator_traits<InputMatrixIterator>::row_iterator
	       row_iterator;

	   row_iterator top (*mat++);
	   row_iterator middle (*mat++);
	   row_iterator bottom (*mat);

       }	   

    template <typename InputMatrixIterator, typename T>
	T det16 (InputMatrixIterator mat)
       {
       }	   

    //=============================================================================
    // Find the inverse matrix
    template <typename InputMatrixIterator, typename OutputLinearIterator>
	OutputLinearIterator inverse
       (InputMatrixIterator mat, OutputLinearIterator res)
       {
	   switch (matrix_iterator_traits<InputMatrixIterator>::size)
	   {
	       case 4:
		   break;

	       case 9:
	       case 16:
	       default:
		   throw std::logic_error ("generic inverse not implemented");
	   };

	   return res;
       }	   
    
    template <typename InputMatrixIterator, typename OutputLinearIterator>
	OutputLinearIterator inverse4
       (InputMatrixIterator mat, OutputLinearIterator res)
       {
	   typename matrix_iterator_traits<InputLinearIterator>::base_value_type
	       det (0);

	   return res;
       }	   
       */
    


    //=============================================================================
    // Makes a Diagonal matrix from a vector
    template <typename InputLinearIterator, typename OutputMatrixIterator>
	OutputMatrixIterator diagonal 
	(InputLinearIterator in, InputLinearIterator end, 
	 OutputMatrixIterator res)
	{
	    typename matrix_iterator_traits<OutputMatrixIterator>::value_type 
		init (0);
	    
	    typename matrix_iterator_traits<OutputMatrixIterator>::difference_type
		col (0);

	    std::fill (res, res + OutputMatrixIterator::size, init);
	    for (; in != end; ++in, ++res, ++col)
		*(*res + col) = *in;

	    return res;
	}

    //-----------------------------------------------------------------------------

    //=============================================================================
    // Order comparible
    template <typename LHSComparable, typename RHSComparable>
	inline bool operator ==
	(const LHSComparable& lhs, const RHSComparable& rhs)
	{ return lhs.operator == (rhs); }

    template <typename LHSComparable, typename RHSComparable>
	inline bool operator !=
	(const LHSComparable& lhs, const RHSComparable& rhs)
	{ return !(lhs.operator == (rhs)); }

    template <typename LHSComparable, typename RHSComparable>
	inline bool operator <
	(const LHSComparable& lhs, const RHSComparable& rhs)
	{ return lhs.operator < (rhs); }

    template <typename LHSComparable, typename RHSComparable>
	inline bool operator >
	(const LHSComparable& lhs, const RHSComparable& rhs)
	{ return rhs.operator < (lhs); }

    template <typename LHSComparable, typename RHSComparable>
	inline bool operator <=
	(const LHSComparable& lhs, const RHSComparable& rhs)
	{ return !(rhs.operator < (lhs)); }

    template <typename LHSComparable, typename RHSComparable>
	inline bool operator >=
	(const LHSComparable& lhs, const RHSComparable& rhs)
	{ return !(lhs.operator < (rhs)); }

    //=============================================================================
    // Misc
    template <typename InputIterator, typename OStream>
	void print (InputIterator iter, InputIterator end, OStream& out)
	{
	    for (; iter != end; ++iter)
		out << *iter << ' ';
	    out << std::endl;
	}
}

#endif //_LINOPERATOR_HPP_
