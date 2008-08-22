/* triangle.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <types.h>
#include <linear/liniterator.hpp>
#include <linear/linoperator.hpp>

namespace ThreeDInc
{
    //=============================================================================
    // Get one point 
    template <typename IndexType, typename VertexIter1, typename VertexIter2>
	VertexIter2 get_point 
	(IndexType index, 
	 VertexIter1 vertexlist,
	 VertexIter2 point)
	{
	    VertexIter1 p (vertexlist + VEC3_SIZE * index);
	    *point++ = *p++;
	    *point++ = *p++;
	    *point = *p;

	    return point;
	}

    //=============================================================================
    // Get one triangle 
    template <typename IndexType, typename VertexIter1, typename VertexIter2>
	VertexIter2 get_triangle 
	(IndexType index1, 
	 IndexType index2, 
	 IndexType index3,
	 VertexIter1 vertexlist,
	 VertexIter2 triangle)
	{
	    get_point (index1, vertexlist, triangle + 0*VEC3_SIZE);
	    get_point (index2, vertexlist, triangle + 1*VEC3_SIZE);
	    get_point (index3, vertexlist, triangle + 2*VEC3_SIZE);

	    return triangle;
	}

    //=============================================================================
    // Get one triangle 
    template <typename IndexIter, typename VertexIter1, typename VertexIter2>
	VertexIter2 get_triangle 
	(IndexIter indexlist, 
	 VertexIter1 vertexlist,
	 VertexIter2 triangle)
	{
	    get_point (*indexlist++, vertexlist, triangle + 0*VEC3_SIZE);
	    get_point (*indexlist++, vertexlist, triangle + 1*VEC3_SIZE);
	    get_point (*indexlist, vertexlist, triangle + 2*VEC3_SIZE);

	    return triangle;
	}

    //=============================================================================
    // Get the normal for a triangle
    template <typename IndexIter, typename VertexIter1, typename VertexIter2>
	VertexIter2 get_polygon_normal
	(IndexIter indexlist_begin, 
	 IndexIter indexlist_end,
	 VertexIter1 vertexlist,
	 VertexIter2 normal)
	{
	    typename linear_iterator_traits<IndexIter>::value_type 
		i1, i2, i3;
	    
	    typename linear_iterator_traits<IndexIter>::difference_type 
		num_ind (indexlist_end - indexlist_begin);
	    
	    // find the normal for the polygon
	    bool valid = false;
	    int a, b, c;
	    
	    // assumes polygon is planar
	    while (!valid)
	    {
		valid = true;

		// randomly pick 3 distinct vertices in the correct winding
		a = rand() % (num_ind/TRI_SIZE);
		b = a+1 + (rand() % (num_ind/TRI_SIZE));
		c = b+1 + (rand() % (num_ind/TRI_SIZE));

		i1 = *(indexlist_begin + a);
		i2 = *(indexlist_begin + b);
		i3 = *(indexlist_begin + c);

		try { get_triangle_normal (i1, i2, i3, vertexlist, normal); }
		catch (divide_by_zero) { valid = false; }
	    } 
	    
	    return normal;
	}

    //=============================================================================
    // Get the normal for a triangle
    template <typename VertexIter, typename NormalIter>
	NormalIter get_triangle_normal
	(VertexIter triangle,
	 NormalIter normal)
	{
	    typename linear_iterator_traits<VertexIter>::value_type 
		data [2*VEC3_SIZE];

	    VertexIter p1 (triangle + 0*VEC3_SIZE);
	    VertexIter p2 (triangle + 1*VEC3_SIZE);
	    VertexIter p3 (triangle + 2*VEC3_SIZE);

	    VertexIter line1 (data + 0*VEC3_SIZE);
	    VertexIter line2 (data + 1*VEC3_SIZE);
	    
	    sub (p2, p2+VEC3_SIZE, p1, line1);
	    sub (p3, p3+VEC3_SIZE, p1, line2);
	    cross (line1, line2, normal);
	    normalize (normal, normal+VEC3_SIZE);

	    return normal;
	}

    //=============================================================================
    // Get the normal for a triangle
    template <typename IndexType, typename VertexIter, typename NormalIter>
	NormalIter get_triangle_normal
	(IndexType index1, 
	 IndexType index2, 
	 IndexType index3,
	 VertexIter vertexlist,
	 NormalIter normal)
	{
	    typename linear_iterator_traits<VertexIter>::value_type 
		triangle [TRI_SIZE*VEC3_SIZE];
	    
	    get_triangle (index1, index2, index3, vertexlist, triangle);
	    get_triangle_normal (triangle, normal);

	    return normal;
	}

    //=============================================================================
    // Get the normal for a triangle
    template <typename IndexIter, typename VertexIter, typename NormalIter>
	NormalIter get_triangle_normal 
	(IndexIter indexlist, 
	 VertexIter vertexlist,
	 NormalIter normal)
	{
	    typename linear_iterator_traits<VertexIter>::value_type 
		triangle [TRI_SIZE*VEC3_SIZE];

	    get_triangle (indexlist, vertexlist, triangle);
	    get_triangle_normal (triangle, normal);

	    return normal;
	}

    //=============================================================================
    // Get the normals for a list of triangle
    template <typename IndexIter, typename VertexIter, typename NormalIter>
	NormalIter get_triangle_normals
	(IndexIter indexlist_begin, 
	 IndexIter indexlist_end, 
	 VertexIter vertexlist,
	 NormalIter normallist)
	{
	    typename linear_iterator_traits<VertexIter>::value_type 
		triangle [TRI_SIZE*VEC3_SIZE];

	    IndexIter ind (indexlist_begin);
	    NormalIter norm (normallist);

	    for (; ind != indexlist_end; ind += TRI_SIZE, norm += VEC3_SIZE)
	    {
		get_triangle (ind, vertexlist, triangle);
		get_triangle_normal (triangle, norm);
	    }

	    return normallist;
	}


    //=============================================================================
    // Whether a triangle is degenerate using common normal
    // TODO: can use SPO here??
    template <typename VertexIter1, typename VertexIter2>
	bool is_degenerate_triangle 
	(VertexIter1 triangle,
	 VertexIter2 normal)
	{
	    typename linear_iterator_traits<VertexIter1>::value_type 
		trinormal [VEC3_SIZE];

	    get_triangle_normal (triangle, trinormal);
	    
	    if (is_gr_zero (dot (trinormal, trinormal+VEC3_SIZE, normal)))
		return false;
	    else
		return true;
	}


    //=============================================================================
    // Whether a vertex exists within a triangle
    template <typename LinearIter>
	bool is_vertex_in_triangle 
	(LinearIter vertex, 
	 LinearIter triangle, 
	 LinearIter normal)
	{
	    typename linear_iterator_traits<LinearIter>::value_type 
		data [6 * VEC3_SIZE];

	    LinearIter point1 (triangle + 0*VEC3_SIZE);
	    LinearIter point2 (triangle + 1*VEC3_SIZE);
	    LinearIter point3 (triangle + 2*VEC3_SIZE);

	    LinearIter line1 (data + 0*VEC3_SIZE);
	    LinearIter line2 (data + 1*VEC3_SIZE);
	    LinearIter line3 (data + 2*VEC3_SIZE);
	    
	    LinearIter cross1 (data + 3*VEC3_SIZE);
	    LinearIter cross2 (data + 4*VEC3_SIZE);
	    LinearIter cross3 (data + 5*VEC3_SIZE);

	    sub (point2, point2+VEC3_SIZE, point1, line1);
	    sub (point3, point3+VEC3_SIZE, point2, line2);
	    sub (point1, point1+VEC3_SIZE, point3, line3);

	    cross (normal, line1, cross1);
	    cross (normal, line2, cross2);
	    cross (normal, line3, cross3);

	    sub (vertex, vertex+VEC3_SIZE, point1, line1);
	    sub (vertex, vertex+VEC3_SIZE, point2, line2);
	    sub (vertex, vertex+VEC3_SIZE, point3, line3);

	    if (is_ge_zero (dot (cross1, cross1+VEC3_SIZE, line1)) &&
		is_ge_zero (dot (cross2, cross2+VEC3_SIZE, line2)) &&
		is_ge_zero (dot (cross3, cross3+VEC3_SIZE, line3)))
		return true;
	    else
		return false;
	}
    
    //=============================================================================
    // Decompose polygon into triangles
    template <typename IndexIter, typename VertexIter>
	IndexIter decompose_polygon 
	(IndexIter indexlist_begin, 
	 IndexIter indexlist_end,
	 VertexIter vertexlist,
	 IndexIter triangle_indexlist)
	{
	    typedef typename linear_iterator_traits<VertexIter>::value_type 
		VertexType;
	    
	    typedef typename linear_iterator_traits<IndexIter>::value_type 
		IndexType;

	    typedef typename linear_iterator_traits<IndexIter>::difference_type 
		DiffType;

	    DiffType num_ind (indexlist_end - indexlist_begin);
	    DiffType num_triangles (0);
	    
	    // Protect against infinite recursion
	    static size_t num_tries = 0;
	    if (++num_tries == num_ind)
		throw std::runtime_error 
		    ("unable to triangulate polygon");

	    // Initialize data
	    VertexType data [5 * VEC3_SIZE]; 
	    VertexIter vertex (data + 0*VEC3_SIZE);
	    VertexIter normal (data + 1*VEC3_SIZE);
	    VertexIter triangle (data + 2*VEC3_SIZE);

	    IndexIter indbegin (indexlist_begin);
	    IndexIter indend (indexlist_end);

	    IndexIter triind (triangle_indexlist);

	    indend -= 1;
	    IndexType back2 (*indend--); 
	    IndexType back1 (*indbegin++);  
	    IndexType fore1 (*indbegin++); 
	    IndexType fore2 (*indbegin++);  

	    // find the normal for the polygon
	    get_polygon_normal 
		(indexlist_begin, indexlist_end, 
		 vertexlist, normal);
	    
	    // direction to walk about the polygon
	    int direction = 1;
	    bool fail = false;

	    // for all indices
	    while (num_triangles < num_ind-2)
	    {
		if (direction > 0)
		{
		    get_triangle (back1, fore1, fore2, vertexlist, triangle);
		    get_point (back2, vertexlist, vertex);
		}
		else
		{
		    get_triangle (back2, back1, fore1, vertexlist, triangle);
		    get_point (fore2, vertexlist, vertex);
		}

		// check if the triangle is non-degenerate, and doesnt
		// contain the remaining vertex -- if so take it
		if (!is_degenerate_triangle (triangle, normal) && 
		    (!is_vertex_in_triangle (vertex, triangle, normal) || 
		     (fore2 == back2)))
		{
		    if (direction > 0)
		    {
			*triind++ = back1;
			*triind++ = fore1;
			*triind++ = fore2;

			// advance along the polygon in the "fore" direction
			fore1 = fore2;
			fore2 = *indbegin++;
		    }
		    else
		    {
			*triind++ = back2;
			*triind++ = back1;
			*triind++ = fore1;

			// advance along the polygon in the "back" direction
			back1 = back2;
			back2 = *indend--;
		    }

		    fail = false;
		    ++ num_triangles;
		}
		else
		{
		    // if we failed in both directions
		    if (fail) break;

		    // change direction and do-over
		    direction *= -1;
		    fail = true;
		}
	    }

	    if (fail)
	    {
		// rotate the indices and try again
		std::rotate (indexlist_begin, indexlist_begin+1, indexlist_end);

		return decompose_polygon 
		    (indexlist_begin, indexlist_end, 
		     vertexlist, triangle_indexlist);
	    }
	    else
		return triangle_indexlist;
	}
}
#endif //_TRIANGLE_H_
