/* main.cpp -- main module
 *
 *			Ryan McDougall -- 20070101
 */

#include <iterator>
#include <vrml.h>
#include <graphics/triangle.hpp>

//=============================================================================
//
namespace ThreeDInc {

void VRMLSceneLoader::close_file () 
{ 
    if (file_.is_open()) 
	file_.close(); 
}

void VRMLSceneLoader::open_file 
(const char *filename)
{
    if (filename)
	filename_ = filename;

    if (file_.is_open())
	close_file();

    if (!filename_.empty())
	file_.open (filename_.c_str(), std::ifstream::in);

    if (!file_)
	throw std::runtime_error
	    ("unable to open file " + filename_);

    // set stream flags
    file_.setf (std::ios::skipws);
}

object_list VRMLSceneLoader::get_objects ()
{
    if (!root_)
    {
	if (!file_.is_open())
	    open_file();

	parse_ (file_);
    }

    return walk_scene_for_objects_ (root_);
}

void VRMLSceneLoader::parse_ 
(std::istream& in)
{
    std::stringstream streambuf;
    streambuf.setf (std::ios::skipws);

    parse_header_ (in);

    if ((headername_ != VRMLConstants::vrml_name) ||
	((headerversion_ != VRMLConstants::vrml_version) &&
	 (headerversion_ != ("V" + VRMLConstants::vrml_version))))
	throw std::runtime_error
	    ("unable to parse" + filename_ + 
	     " of type " + headername_ + " " + headerversion_);

    stripcomments (in, streambuf);
    //log_debug (log_, "stripped file: " << streambuf.str());

    root_ = new VRMLParseObjects::Separator (streambuf);
}

void VRMLSceneLoader::parse_header_ 
(std::istream& in)
{
    int charbuf;

    std::stringstream streambuf;
    streambuf.setf (std::ios::skipws);

    in >> std::ws;

    getline (in, streambuf);
    charbuf = streambuf.get();

    if (charbuf == VRMLConstants::comment)
	streambuf >> headername_ >> headerversion_;

    else throw std::runtime_error 
	("unable to parse header " + streambuf.str());
}

object_list VRMLSceneLoader::get_objects_ 
(VRMLParseObjects::Separator *group)
{
    VRMLParseObjects::scene_list::iterator iter;
    VRMLParseObjects::scene_list::iterator end;

    object_list objects;
    index_list indicies;
    vertex_list vertices;
    normal_list normals;

    bool have_vertices = false;
    bool have_indices = false;

    for (iter = group->children.begin(), 
	 end = group->children.end(); 
	 iter != end; ++iter)
    {
	if (is_material (*iter)) 
	{
	    log_debug (log(), "Material");
	    VRMLParseObjects::Material *obj
		= static_cast <VRMLParseObjects::Material*> (*iter);
	}

	else if (is_coordinate3 (*iter)) 
	{
	    if (have_vertices)
	    {
		add_object_ (indicies, vertices, normals, objects);
		have_vertices = false;
		have_indices = false;
	    }

	    log_debug (log(), "Coordinate3");

	    VRMLParseObjects::Coordinate3 *obj
		= static_cast <VRMLParseObjects::Coordinate3*> (*iter);

	    VRMLParseObjects::point_list::iterator i;
	    VRMLParseObjects::point_list::iterator end;

	    for (i = obj->points.begin(),
		 end = obj->points.end(); 
		 i != end; ++i)
	    {
		vertices.push_back (i->x);
		vertices.push_back (i->y);
		vertices.push_back (i->z);
	    }

	    have_vertices = true;
	}

	else if (is_indexed_face_set (*iter)) 
	{
	    if (have_indices)
	    {
		add_object_ (indicies, vertices, normals, objects);
		have_vertices = false;
		have_indices = false;
	    }

	    log_debug (log(), "IndexedFaceSet");

	    VRMLParseObjects::IndexedFaceSet *obj
		= static_cast <VRMLParseObjects::IndexedFaceSet*> (*iter);

	    indicies.insert 
		(indicies.end(), 
		 obj->coordIndicies.begin(), 
		 obj->coordIndicies.end());

	    normals.insert 
		(normals.end(), 
		 obj->normalIndicies.begin(), 
		 obj->normalIndicies.end());

	    have_indices = true;
	}
    }

    if (have_vertices && have_indices)
	add_object_ (indicies, vertices, normals, objects);

    return objects;
}

object_list VRMLSceneLoader::walk_scene_for_objects_ 
(VRMLParseObjects::Separator *branch)
{
    object_list objects;

    VRMLParseObjects::scene_list::iterator iter, end;

    for (iter = branch->children.begin(), 
	 end = branch->children.end(); 
	 iter != end; ++iter)
    {
	if (is_separator (*iter)) 
	{
	    object_list ret 
		(walk_scene_for_objects_ 
		 (static_cast<VRMLParseObjects::Separator*> (*iter)));

	    objects.insert (objects.end(), ret.begin(), ret.end());
	}
    }

    object_list branch_objects (get_objects_ (branch));

    objects.insert 
	(objects.begin(), 
	 branch_objects.begin(), branch_objects.end());

    return objects;
}

void VRMLSceneLoader::add_object_ 
(index_list& indices, vertex_list& vertices, 
 normal_list& normals, object_list& objects)
{
    log_debug (log(), "building object: " << vertices.size() << ' ' << indices.size());
    index_list decomposed_indices;
    vertex_list decomposed_vertices;
    normal_list decomposed_normals;

    print (vertices.begin(), vertices.end(), std::cout);

    // TODO: normal calc proceedure depends on input normals, and shading requirements
    if (vertices.size() && indices.size())
    {
	// decompose polygons into triangles (and remove -1s delims)
	decomposed_indices = decompose_object_ (indices, vertices);

	// recalculate normals
	if (normals.empty())
	{
	    vertex_t triangle [TRI_SIZE*VEC3_SIZE];
	    normal_t normal [VEC3_SIZE];

	    // less typing
	    std::back_insert_iterator <vertex_list> vlist (decomposed_vertices);
	    std::back_insert_iterator <normal_list> nlist (decomposed_normals);

	    // copy out normal+vertex pairs for each triangle
	    index_list::iterator ind (decomposed_indices.begin());
	    index_list::iterator end (decomposed_indices.end());

	    // TODO: implement some sort of vertex+normal sharing!
	    for (; ind != end; ind += TRI_SIZE)
	    {
		get_triangle (ind, vertices.begin(), triangle);
		get_triangle_normal (triangle, normal);

		for (int i=0; i < TRI_SIZE; ++i)
		    for (int j=0; j < VEC3_SIZE; ++j)
		    {
			*vlist = triangle[i*TRI_SIZE+j];
			*nlist = normal[j];
		    }
	    }
	}

	else if (normals.size() == vertices.size())
	{
	    // smooth shading, presume input is correct
	    decomposed_vertices = vertices;
	    decomposed_normals = normals;
	}

	else throw not_implemented_error 
	    ("must recalculate smooth shaded normals for decomposed shape");

	// create new object
	objects.push_back 
	    (new Object 
	     (decomposed_vertices, 
	      decomposed_normals, 
	      decomposed_indices));

	indices.clear();
	vertices.clear();
	normals.clear();
    }

    else throw std::runtime_error 
	("unable to build an object from the parse tree");

}

index_list VRMLSceneLoader::decompose_object_
(index_list& indices, vertex_list& vertices)
{
    index_list decomp_indices;
    index_list poly_indices;
    index_list tmp;

    index_list::iterator end (indices.end());
    index_list::iterator iter (indices.begin());
    index_list::iterator polybegin (iter);

    size_t numtri = 0;

    for (; iter != end; ++iter)
    {
	if (*iter < 0)
	{
	    // get expected num triangles
	    numtri = 3 * ((iter - polybegin) - 2);
	    
	    // clear the tmp buffer and alloc room
	    tmp.clear();
	    tmp.insert (tmp.begin(), numtri, 0);
	    
	    // copy the polygon's indices
	    poly_indices.assign (polybegin, iter);

	    // decompose
	    decompose_polygon 
		(poly_indices.begin(), poly_indices.end(), 
		 vertices.begin(), tmp.begin());

	    // add triangle indices to the main lsit
	    decomp_indices.insert (decomp_indices.end(), tmp.begin(), tmp.end());

	    // move to the next polygon
	    polybegin = iter+1;
	}
    }

    if (polybegin == indices.begin())
	return indices;
    else
	return decomp_indices;
}

}
