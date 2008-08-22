/* types.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

#include <stdexcept>
#include <vector>
#include <tr1/array>

namespace ThreeDInc
{
    struct Object;

    typedef double real_t;
    typedef real_t vertex_t;
    typedef real_t normal_t;
    typedef real_t color_t;
    typedef int index_t;
    typedef unsigned long hash_t;

    typedef std::vector<vertex_t> vertex_list;
    typedef std::vector<normal_t> normal_list;
    typedef std::vector<index_t> index_list;

    typedef std::vector<Object*> object_list;

    class VRMLSceneLoader;

    namespace VRMLParseObjects
    {
	class Vector;
	class Color;
	class Base;
	class Material; 
	class Coordinate3;
	class IndexedFaceSet;
	class Separator;

	typedef long int MFLong;
	typedef float MFFloat;
	typedef Color MFColor;
	typedef Vector MFVec3f;

	typedef std::vector<Base*> scene_list;
	typedef std::vector<MFVec3f> point_list;
	typedef std::vector<MFLong> index_list;
    }

    static const size_t VEC2_SIZE = 2;
    static const size_t VEC3_SIZE = 3;
    static const size_t VEC4_SIZE = 4;
    static const size_t COLOR_SIZE = 4;
    static const size_t MATRIX2_SIZE = 2 * 2;
    static const size_t MATRIX3_SIZE = 3 * 3;
    static const size_t MATRIX4_SIZE = 4 * 4;
    static const size_t TRI_SIZE = 3;

    typedef std::tr1::array <real_t, VEC3_SIZE> vec3;
    typedef std::tr1::array <real_t, VEC4_SIZE> vec4;
    
    class not_implemented_error : public std::runtime_error 
    {
	public:
	    explicit not_implemented_error (const std::string& s) : 
		std::runtime_error (s) {}

	    virtual const char* what() const throw() 
	    { return std::runtime_error::what(); }
	
	private:
	    not_implemented_error ();
    };

}

#endif //_TYPES_H_
