/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _LINOPERATOR_HPP_
#define _LINOPERATOR_HPP_

#include <stdexcept>
#include <functional>
#include <numeric>
#include <cmath>

#include <linear/liniterator.hpp>
#include <linear/matrix.hpp>

namespace ThreeDInc
{
    class divide_by_zero : public std::runtime_error 
    {
	public:
	    divide_by_zero () : 
		std::runtime_error ("divide by zero") {}

	    explicit divide_by_zero (const std::string& s) : 
		std::runtime_error (s) {}

	    virtual const char* what() const throw() 
	    { return std::runtime_error::what(); }
    };

    //=============================================================================
    // Test int is even
    inline bool is_even (int n)
    { 
	if (n < 0) n = -n;
	return (n & 1)? false : true;
    }

    //=============================================================================
    // comparisons with epsilon
    static const double eps_ (1e-10);

    // Test float for zero
    inline bool is_zero (double n) { return (std::fabs (n) < eps_); }

    // Test float for greater than or equal with epsilon
    inline bool is_ge_zero (double n) { return (n > -eps_); }

    // Test float for less than or equal with epsilon
    inline bool is_le_zero (double n) { return (n < eps_); }

    // Test float for greater than or equal with epsilon
    inline bool is_gr_zero (double n) { return (n > eps_); }

    // Test float for less than or equal with epsilon
    inline bool is_ls_zero (double n) { return (n < -eps_); }

    //=============================================================================
    // Sign of a float
    inline double sign (double a, double b)
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
	OutputMatrix transpose 
	(InputMatrix A, OutputMatrix B)
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
    template <typename InputLinearIter1, typename InputLinearIter2, typename OutputLinearIter>
	OutputLinearIter add 
	(InputLinearIter1 lhs, 
	 InputLinearIter1 end, 
	 InputLinearIter2 rhs, 
	 OutputLinearIter res)
	{
	    typedef typename linear_iterator_traits<InputLinearIter1>::value_type T;

	    std::transform (lhs, end, rhs, res, std::plus <T>());
	    return res;
	}

    //=============================================================================
    // Vector and Matrix Subtraction
    template <typename InputLinearIter1, typename InputLinearIter2, typename OutputLinearIter>
	OutputLinearIter sub 
	(InputLinearIter1 lhs, 
	 InputLinearIter1 end, 
	 InputLinearIter2 rhs, 
	 OutputLinearIter res)
	{
	    typedef typename linear_iterator_traits<InputLinearIter1>::value_type T;

	    std::transform (lhs, end, rhs, res, std::minus <T>());
	    return res;
	}

    //=============================================================================
    // Vector and Matrix - Scalar Multiplication
    template <typename InputLinearIter, typename OutputLinearIter, typename T>
	OutputLinearIter mul 
	(InputLinearIter lhs, 
	 InputLinearIter end, 
	 OutputLinearIter res, 
	 T scale)
	{
	    std::transform (lhs, end, res, 
		    std::bind1st (std::multiplies <T>(), scale));

	    return res;
	}

    //=============================================================================
    // Vector and Matrix - Scalar Division
    template <typename InputLinearIter, typename OutputLinearIter, typename T>
	OutputLinearIter div 
	(InputLinearIter lhs, 
	 InputLinearIter end, 
	 OutputLinearIter res, 
	 T scale)
	{
	    std::transform (lhs, end, res, 
		    std::bind1st (std::divides <T>(), scale));

	    return res;
	}

    //=============================================================================
    // Vector dot product
    template <typename LinearIter1, typename LinearIter2>
	typename linear_iterator_traits<LinearIter2>::value_type 
	dot
	(LinearIter1 lhs, 
	 LinearIter1 end, 
	 LinearIter2 rhs)
	{ 
	    typename linear_iterator_traits<LinearIter1>::value_type 
		init (0);
	    
	    return std::inner_product (lhs, end, rhs, init); 
	}

    //=============================================================================
    // Vector cross product
    template <typename InputLinearIter, typename OutputLinearIter>
	OutputLinearIter cross
	(InputLinearIter lhs, 
	 InputLinearIter rhs, 
	 OutputLinearIter res)
	{
	    *res++ = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	    *res++ = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	    *res = lhs[0] * rhs[1] - lhs[1] * rhs[0];

	    return res;
	}

    //=============================================================================
    // Vector norm
    template <typename LinearIter>
	typename linear_iterator_traits<LinearIter>::value_type
       	norm
	(LinearIter begin, 
	 LinearIter end) 
	{ 
	    typedef typename linear_iterator_traits<LinearIter>::value_type T;

	    return sqrt (std::inner_product (begin, end, begin, T(0))); 
	}

    //=============================================================================
    // Vector normalization
    template <typename LinearIter>
	void normalize
	(LinearIter begin, 
	 LinearIter end) 
	{ 
	    typedef typename linear_iterator_traits<LinearIter>::value_type T;

	    T n (norm (begin, end));
	    if (!is_zero (n))
	    {
		n = T(1.0) / n;
		mul (begin, end, begin, n);
	    }
	    else
		throw divide_by_zero();
	}

    //=============================================================================
    // Cartesian Product
    template <typename InputLinearIter1, typename InputLinearIter2, typename OutputLinearIter>
	OutputLinearIter cartesian 
	(InputLinearIter1 lhs, InputLinearIter1 lend,
	 InputLinearIter2 rhs, InputLinearIter2 rend,
	 OutputLinearIter res)
	{
	    InputLinearIter2 rptr (rhs);
	    for (; lhs != lend; ++lhs, rptr = rhs)
		for (; rptr != rend; ++rptr, ++res)
		    *res = *lhs * *rptr;

	    return res;
	}

    //=============================================================================
    // Matrix - Matrix Product
    template <typename InputMatrixIter1, typename InputMatrixIter2, 
	     typename OutputLinearIter>
	OutputLinearIter mul_rowmajor
	(InputMatrixIter1 lhs, InputMatrixIter1 lhsend,
	 InputMatrixIter2 rhs, InputMatrixIter2 rhsend,
	 OutputLinearIter res)
	{
	    typename matrix_iterator_traits<InputMatrixIter1>::value_type 
		init (0);
	    
	    typename matrix_iterator_traits<InputMatrixIter1>::size_type 
		lhsoff (matrix_iterator_traits<InputMatrixIter1>::line_offset);

	    typename matrix_iterator_traits<InputMatrixIter2>::size_type 
		rhsoff (matrix_iterator_traits<InputMatrixIter2>::line_offset);

	    InputMatrixIter1 row (lhs);
	    InputMatrixIter2 col (rhs);

	    for (; row != lhsend; row += lhsoff, col = rhs)
		for (; col != rhsend; col += rhsoff, ++res)
		    *res = std::inner_product (row, row + lhsoff, col, init);

	    return res;
	}

    //=============================================================================
    // Matrix - Matrix Product
    template <typename InputMatrixIter1, typename InputMatrixIter2, 
	     typename OutputLinearIter>
	OutputLinearIter mul 
	(InputMatrixIter1 lhs, InputMatrixIter1 lhsend,
	 InputMatrixIter2 rhs, InputMatrixIter2 rhsend,
	 OutputLinearIter res)
	{
	    typename matrix_iterator_traits<InputMatrixIter1>::base_value_type 
		init (0);
	    
	    typename matrix_iterator_traits<InputMatrixIter1>::size_type 
		ncol (matrix_iterator_traits<InputMatrixIter1>::line_offset);

	    InputMatrixIter1 row (lhs);
	    InputMatrixIter2 col (rhs);

	    for (; row != lhsend; ++row, col = rhs)
		for (; col != rhsend; ++col, ++res)
		    *res = std::inner_product (*row, *row + ncol, *col, init);

	    return res;
	}

    //=============================================================================
    // Find the determinant
    /*
    template <typename InputMatrixIter, typename T>
	T det (InputMatrixIter mat)
       {
	   T res (0);

	   switch (matrix_iterator_traits<InputMatrixIter>::size)
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
    
    template <typename InputMatrixIter, typename T>
	T det4 (InputMatrixIter mat)
       {
	   typedef typename matrix_iterator_traits<InputMatrixIter>::row_iterator
	       row_iterator;

	   row_iterator top (*mat++);
	   row_iterator bottom (*mat);

	   return top[0] * bottom[1] - top[1] * bottom[0];
       }	   
    
    template <typename InputMatrixIter, typename T>
	T det9 (InputMatrixIter mat)
       {
	   typedef typename matrix_iterator_traits<InputMatrixIter>::row_iterator
	       row_iterator;

	   row_iterator top (*mat++);
	   row_iterator middle (*mat++);
	   row_iterator bottom (*mat);

       }	   

    template <typename InputMatrixIter, typename T>
	T det16 (InputMatrixIter mat)
       {
       }	   

    //=============================================================================
    // Find the inverse matrix
    template <typename InputMatrixIter, typename OutputLinearIter>
	OutputLinearIter inverse
       (InputMatrixIter mat, OutputLinearIter res)
       {
	   switch (matrix_iterator_traits<InputMatrixIter>::size)
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
    
    template <typename InputMatrixIter, typename OutputLinearIter>
	OutputLinearIter inverse4
       (InputMatrixIter mat, OutputLinearIter res)
       {
	   typename matrix_iterator_traits<InputLinearIter>::base_value_type
	       det (0);

	   return res;
       }	   
       */
    


    //=============================================================================
    // Makes a Diagonal matrix from a vector
    template <typename InputLinearIter, typename OutputMatrixIter>
	OutputMatrixIter diagonal 
	(InputLinearIter in, 
	 InputLinearIter end, 
	 OutputMatrixIter res)
	{
	    typename matrix_iterator_traits<OutputMatrixIter>::value_type 
		init (0);
	    
	    typename matrix_iterator_traits<OutputMatrixIter>::difference_type
		col (0);

	    std::fill (res, res + OutputMatrixIter::size, init);
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
    template <typename InputIter, typename OStream>
	void print (InputIter iter, InputIter end, OStream& out)
	{
	    for (; iter != end; ++iter)
		out << *iter << ' ';
	    out << std::endl;
	}
}

#endif //_LINOPERATOR_HPP_
