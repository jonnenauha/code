/* object.cpp -- object module
 *
 *			Ryan McDougall -- 20070101
 */

#include <ostream>
#include <object.h>

namespace ThreeDInc {

std::ostream& operator<< 
(std::ostream& out, Object& o)
{
    out << "indicies: ";
    for (const index_t *p = o.index_ptr(); 
	 p < o.index_ptr()+o.num_index; ++p)
	out << *p << ' ';
    out << ',';

    out << "vertices: ";
    for (const vertex_t *p = o.vertex_ptr(); 
	 p < o.vertex_ptr()+o.num_vertex; ++p)
	out << *p << ' ';
    out << ',';

    out << "normals: ";
    for (const normal_t *p = o.normal_ptr(); 
	 p < o.normal_ptr()+o.num_normal; ++p)
	out << *p << ' ';

    return out;
}

}
