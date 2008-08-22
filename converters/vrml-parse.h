/* vrml-parse.h -- exported function header
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _VRML_PARSE_H_
#define _VRML_PARSE_H_

#include <iostream> 
#include <types.h>
#include <util/text.h>

namespace ThreeDInc
{
    namespace VRMLParseObjects
    {
	std::ostream& operator<< (std::ostream& out, const Vector& obj);
	std::ostream& operator<< (std::ostream& out, const Color& obj);
	std::ostream& operator<< (std::ostream& out, const Coordinate3& obj);
	std::ostream& operator<< (std::ostream& out, const Material& obj);
	std::ostream& operator<< (std::ostream& out, const IndexedFaceSet& obj);

	std::istream& operator>> (std::istream& in, Vector& obj);
	std::istream& operator>> (std::istream& in, Color& obj);
	std::istream& operator>> (std::istream& in, Material& obj);
	std::istream& operator>> (std::istream& in, Coordinate3& obj);
	std::istream& operator>> (std::istream& in, IndexedFaceSet& obj);

	struct Vector 
	{
	    Vector() : x (0), y (0), z (0) {}
	    Vector (std::istream& in) { in >> x >> y >> z; }

	    MFFloat x;
	    MFFloat y;
	    MFFloat z;
	};

	struct Color
	{
	    Color() : red (0), green (0), blue (0) {}
	    Color (std::istream& in) { in >> red >> green >> blue; }

	    MFFloat red;
	    MFFloat green;
	    MFFloat blue;
	};

	enum discrim_t
	{
	    base_type,
	    material_type,
	    coordinate3_type,
	    indexed_face_set_type,
	    separator_type,
	};

	class Base
	{
	    public: 
		Base (std::istream& in) : discriminator (-1) { parse_ (in); }
		Base (std::istream& in, discrim_t d) : discriminator (d) { parse_ (in); }

		int discriminator;

	    private:
		void parse_ (std::istream& in);

	    private:
		Base();
	};

	class Material : 
	    public Base, public Loggable <Material>
	{
	    public:
		Material (std::istream& in) :
		    Base (in, material_type), shininess (0), transparency (0) 
		{ parse_ (in); }

		MFColor ambientColor;
		MFColor diffuseColor;
		MFColor specularColor;
		MFColor emissiveColor;
		MFFloat shininess;
		MFFloat transparency;

	    private:
		void parse_ (std::istream& in);

	    private:
		Material();
	};

	class Coordinate3 : 
	    public Base, public Loggable <Coordinate3>
	{
	    public:
		Coordinate3 (std::istream& in) :
		    Base (in, coordinate3_type) { parse_ (in); }

		point_list points;

	    private:
		void parse_ (std::istream& in);

	    private:
		Coordinate3();
	};

	class IndexedFaceSet : 
	    public Base, public Loggable <IndexedFaceSet>
	{
	    public:
		IndexedFaceSet (std::istream& in) :
		    Base (in, indexed_face_set_type) { parse_ (in); }

		index_list coordIndicies;
		index_list materialIndicies;
		index_list normalIndicies;
		index_list textureCoordIndicies;

		void parse_ (std::istream& in);
		void parse_index_list_ (std::istream& in, index_list& list);

	    private:
		IndexedFaceSet();
	};
	
	class Separator :
	    public Base, Loggable <Separator>
	{
	    public:
		Separator (std::istream& in) :
		    Base (in, separator_type), depth_ (0) { parse_ (in); }

		Separator (std::istream& in, size_t depth) :
		    Base (in, separator_type), depth_ (depth) { parse_ (in); }

		scene_list children;

	    private:

		void parse_ (std::istream& in);
		void parse_object_ (std::istream& in);

	    private:
		size_t depth_;
	    
	    private:
		Separator();
	};
	
	inline bool is_base (const Base *obj) { return (obj->discriminator == base_type); }
	inline bool is_material (const Base *obj) { return (obj->discriminator == material_type); }
	inline bool is_coordinate3 (const Base *obj) { return (obj->discriminator == coordinate3_type); }
	inline bool is_indexed_face_set (const Base *obj) { return (obj->discriminator == indexed_face_set_type); }
	inline bool is_separator (const Base *obj) { return (obj->discriminator == separator_type); }


	std::ostream& operator<< (std::ostream& out, const Vector& obj);
	std::ostream& operator<< (std::ostream& out, const Color& obj);
	std::ostream& operator<< (std::ostream& out, const Coordinate3& obj);
	std::ostream& operator<< (std::ostream& out, const Material& obj);
	std::ostream& operator<< (std::ostream& out, const IndexedFaceSet& obj);
	std::istream& operator>> (std::istream& in, Vector& obj);
	std::istream& operator>> (std::istream& in, Color& obj);
	std::istream& operator>> (std::istream& in, Material& obj);
	std::istream& operator>> (std::istream& in, Coordinate3& obj);
	std::istream& operator>> (std::istream& in, IndexedFaceSet& obj);
    }
}

#endif //_VRML_PARSE_H_
