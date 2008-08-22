/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _FILEOBJLOADER_HPP_
#define _FILEOBJLOADER_HPP_

#include <string>
#include <fstream>
#include <types.h>

/*
static const size_t LINESIZE = 80;

inline void get_triangle_norm 
(const real_t triangle[9], real_t norm[3])
{ normal (triangle, triangle+3, triangle+6, norm); }

inline void get_vertex
(const vertex_list& vert, const index_list& ind, size_t i, vertex_t v[3])
{
    index_t vp = ind[i]*3;
    v[0] = vert[vp+0];
    v[1] = vert[vp+1];
    v[2] = vert[vp+2];
}

inline void get_triangle 
(const vertex_list& vert, const index_list& ind, size_t tri, vertex_t t[9])
{
    get_vertex (vert, ind, tri+0, t+3*0);
    get_vertex (vert, ind, tri+1, t+3*1);
    get_vertex (vert, ind, tri+2, t+3*2);
}

class ObjFileLoader
{
    public:

	ObjFileLoader () {}
	~ObjFileLoader () {}

	void open_file (const std::string& filename)
	{
	    if (_file.is_open())
		_file.close();

	    _file.open (filename.c_str(), std::ifstream::in);

	    if (!_file)
		throw std::runtime_error
		    ("unable to open file " + filename);
	}

	Object* load_object ()
	{
	    char c;
	    real_t x, y, z;
	    index_t i, j, k;

	    vertex_list vertices;
	    index_list indices;
	    
	    vertex_list flat_vertices;
	    index_list flat_indices;
	    normal_list flat_normals;

	    if (!_file.is_open())
		return NULL;

	    _file.setf (std::ios::skipws);

	    while (!_file.eof())
	    {
		x = y = z = 0.0;

		while ((c = _file.peek()) == '#')
		    _file.ignore (LINESIZE, '\n');

		_file >> c;
		switch (c)
		{
		    case 'v':
			_file >> x >> y >> z;
			vertices.push_back (x);
			vertices.push_back (y);
			vertices.push_back (z);
			break;

		    case 'f':
			_file >> i >> j >> k;
			indices.push_back (i-1);
			indices.push_back (j-1);
			indices.push_back (k-1);
			break;

			case '\n':
		    case '\r':
			break;

		    default:
			throw std::runtime_error
			    ("unknown line type");
		}
	    }

	    // Calculate the normals for each face and add to a new vertex list 
	    // for flat shading where vertices on (sharp) edges are duplicated
	    vertex_t triangle[9];
	    normal_t norm[3];
	    
	    for (size_t t=0; t < indices.size(); t += 3)
	    {
		// get each face, and its normal
		get_triangle (vertices, indices, t, triangle);
		get_triangle_norm (triangle, norm);

		// record the triangle as separate unshared vertices
		for (size_t i=0; i < 3; ++i)
		{
		    // vertex index
		    flat_indices.push_back (flat_vertices.size()/3);

		    // vertex x,y,z
		    flat_vertices.push_back (triangle[i*3+0]);
		    flat_vertices.push_back (triangle[i*3+1]);
		    flat_vertices.push_back (triangle[i*3+2]);
		    
		    // vertex normal x,y,z
		    flat_normals.push_back (norm[0]);
		    flat_normals.push_back (norm[1]);
		    flat_normals.push_back (norm[2]);
		}
	    }

	    return new Object (flat_vertices, flat_normals, flat_indices);
	}

    private:
	ObjFileLoader (const ObjFileLoader& copy);
	ObjFileLoader& operator= (const ObjFileLoader& rhs);

    private:
	std::ifstream       _file;
};
*/
#endif //_FILEOBJLOADER_HPP_
