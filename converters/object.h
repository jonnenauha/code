/* object.h -- object definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <types.h>

//=============================================================================
//
namespace ThreeDInc
{
    struct Object
    {
	Object (){}

	Object (const vertex_list& v, const normal_list& n, const index_list& i) :
	    num_vertex (v.size()), 
	    num_normal (n.size()), 
	    num_index (i.size()),
	    vertex (new vertex_t[num_vertex]), 
	    normal (new normal_t[num_normal]), 
	    index (new index_t[num_index])
	{
	    std::copy (v.begin(), v.end(), vertex.get());
	    std::copy (n.begin(), n.end(), normal.get());
	    std::copy (i.begin(), i.end(), index.get());
	}

	const vertex_t* vertex_ptr () const { return vertex.get(); }
	const normal_t* normal_ptr () const { return normal.get(); }
	const index_t* index_ptr () const { return index.get(); }

	size_t			num_vertex;
	size_t			num_normal;
	size_t			num_index;

	std::auto_ptr<vertex_t>	vertex;
	std::auto_ptr<normal_t>	normal;
	std::auto_ptr<index_t>	index;

    };
	
    std::ostream& operator<< (std::ostream& out, Object& o);
}

#endif //_OBJECT_H_
