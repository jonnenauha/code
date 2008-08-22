/* vrml.hpp -- vrml definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _VRML_H_
#define _VRML_H_

#include <fstream>
#include <stdexcept>

#include <vrml-parse.h>
#include <vrml-constants.h>
#include <object.h>
#include <types.h>

//=============================================================================
//
namespace ThreeDInc
{
    class VRMLSceneLoader : public Loggable <VRMLSceneLoader>
    {
	public:
	    VRMLSceneLoader () : 
		root_ (NULL) {}

	    VRMLSceneLoader (const std::string& filename) :
		filename_ (filename), root_ (NULL) {}

	    ~VRMLSceneLoader () { close_file(); }

	    void close_file ();
	    void open_file (const char *filename = NULL);

	    object_list get_objects ();

	private:
	    std::ifstream 	file_;
	    std::string		filename_;
	    std::string		headername_;
	    std::string		headerversion_;

	    VRMLParseObjects::Separator *root_;

	private:
	    void parse_ (std::istream& in);
	    void parse_header_ (std::istream& in);

	    object_list get_objects_ 
		(VRMLParseObjects::Separator *group);

	    object_list walk_scene_for_objects_ 
		(VRMLParseObjects::Separator *branch);
	    
	    void add_object_ 
		(index_list& indicies, vertex_list& vertices, 
		 normal_list& normals, object_list& objects);

	    index_list decompose_object_ 
		(index_list& indicies, vertex_list& vertices);
    };
}

#endif //_VRML_H_
