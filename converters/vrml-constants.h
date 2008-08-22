/* main.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _VRML_CONSTANTS_H_
#define _VRML_CONSTANTS_H_

#include <string>
#include <types.h>
#include <util/text.h>

namespace ThreeDInc
{
    namespace VRMLConstants
    {
	using std::string;

	static const char space (' ');
	static const char newline ('\n');
	static const char comment ('#');
	static const char comma (',');
	
	static const char bracket_left_curl ('{');
	static const char bracket_right_curl ('}');
	static const char bracket_left_sq ('[');
	static const char bracket_right_sq (']');

	static const string vrml_name ("VRML");
	static const string vrml_version ("1.0");
	static const string encoding ("ascii");

	static const string fields ("fields");

	static const string separator ("Separator");
	static const string material ("Material");
	static const string indexed_face_set ("IndexedFaceSet");
	static const string coordinate3 ("Coordinate3");

	static const string ambient_color ("ambientColor");
	static const string diffuse_color ("diffuseColor");
	static const string specular_color ("specularColor");
	static const string emissive_color ("emissiveColor");
	static const string shininess ("shininess");
	static const string transparency ("transparency");
	
	static const string point ("point");
		
	static const string coord_index ("coordIndex");
	static const string material_index ("materialIndex");
	static const string normal_index ("normalIndex");
	static const string texture_coord_index ("textureCoordIndex");

	static const hash_t separator_hash (str_hash_fun (separator));

	static const hash_t material_hash (str_hash_fun (material));
	static const hash_t indexed_face_set_hash (str_hash_fun (indexed_face_set));
	static const hash_t coordinate3_hash (str_hash_fun (coordinate3));

	static const hash_t ambient_color_hash (str_hash_fun (ambient_color));
	static const hash_t diffuse_color_hash (str_hash_fun (diffuse_color));
	static const hash_t specular_color_hash (str_hash_fun (specular_color));
	static const hash_t emissive_color_hash (str_hash_fun (emissive_color));
	static const hash_t shininess_hash (str_hash_fun (shininess));
	static const hash_t transparency_hash (str_hash_fun (transparency));

	static const hash_t point_hash (str_hash_fun (point));
	
	static const hash_t coord_index_hash (str_hash_fun (coord_index));
	static const hash_t material_index_hash (str_hash_fun (material_index));
	static const hash_t normal_index_hash (str_hash_fun (normal_index));
	static const hash_t texture_coord_index_hash (str_hash_fun (texture_coord_index));

    }
}

#endif //_VRML_CONSTANTS_H_
