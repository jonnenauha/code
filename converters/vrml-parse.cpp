/* vrml-parse.cpp
 *
 *			Ryan McDougall -- 20070101
 */

#include <sstream>

#include <vrml-constants.h>
#include <vrml-parse.h>

namespace ThreeDInc {
namespace VRMLParseObjects {
	
void Base::parse_ 
(std::istream& in)
{
    // remove the "fields" section
    std::string fields;
    std::istream::streampos gpos (in.tellg());

    in >> fields;
    if (fields == VRMLConstants::fields)
    {
	in >> std::ws;
	ignorebrackets (in);
    }
    else
	in.seekg (gpos);
}

void Material::parse_ 
(std::istream& in)
{
    std::string param;
    hash_t hash;

    while (!in.eof())
    {
	in >> param;
	hash = str_hash_fun (param);

	if (hash == VRMLConstants::ambient_color_hash)
	    in >> ambientColor;

	else if (hash == VRMLConstants::diffuse_color_hash)
	    in >> diffuseColor;

	else if (hash == VRMLConstants::specular_color_hash)
	    in >> specularColor;

	else if (hash == VRMLConstants::emissive_color_hash)
	    in >> emissiveColor;

	else if (hash == VRMLConstants::shininess_hash)
	    in >> shininess;

	else if (hash == VRMLConstants::transparency_hash)
	    in >> transparency;

	else
	    log_error (log(), "unable to parse " << param);

	in >> std::ws;
    }
}

void Coordinate3::parse_ 
(std::istream& in)
{
    std::string param;
    in >> param >> std::ws;

    if (param != VRMLConstants::point)
	log_error (log(), "unable to parse " << param);

    MFVec3f p;

    int charbuf (in.peek());
    if (charbuf == VRMLConstants::bracket_left_sq)
    {
	std::stringstream streambuf;
	streambuf.setf (std::ios::skipws);

	readbrackets (in, streambuf);

	while (!streambuf.eof())
	{
	    streambuf >> p;

	    if (streambuf.peek() == VRMLConstants::comma)
		streambuf.ignore(); 

	    streambuf >> std::ws;

	    points.push_back (p);
	}
    }

    else
    {
	in >> p >> std::ws;
	points.push_back (p);
    }
}

void IndexedFaceSet::parse_ 
(std::istream& in)
{
    std::string param;
    hash_t hash;

    while (!in.eof())
    {
	in >> param >> std::ws;

	hash = str_hash_fun (param);
	if (hash == VRMLConstants::coord_index_hash)
	    parse_index_list_ (in, coordIndicies);

	else if (hash == VRMLConstants::material_index_hash)
	    parse_index_list_ (in, materialIndicies);

	else if (hash == VRMLConstants::normal_index_hash)
	    parse_index_list_ (in, normalIndicies);

	else if (hash == VRMLConstants::texture_coord_index_hash)
	    parse_index_list_ (in, textureCoordIndicies);

	else
	    log_error (log(), "unable to parse " << param);

	in >> std::ws;
    }
}

void IndexedFaceSet::parse_index_list_ 
(std::istream& in, std::vector<MFLong>& list)
{
    MFLong p;

    int charbuf (in.peek());
    if (charbuf == VRMLConstants::bracket_left_sq)
    {
	std::stringstream streambuf;
	streambuf.setf (std::ios::skipws);

	readbrackets (in, streambuf);

	while (!streambuf.eof())
	{
	    streambuf >> p;

	    if (streambuf.peek() == VRMLConstants::comma)
		streambuf.ignore(); 

	    streambuf >> std::ws;

	    list.push_back (p);
	}
    }

    else 
    {
	in >> p >> std::ws;
	list.push_back (p);
    }
}

void Separator::parse_ 
(std::istream& in)
{
    // parse the file
    int charbuf;
    while (!in.eof())
    {
	parse_object_ (in);
	in >> std::ws;
    }
}

void Separator::parse_object_ 
(std::istream& in)
{
    std::string node;
    std::stringstream streambuf;
    streambuf.setf (std::ios::skipws);

    // read in the node name
    in >> node >> std::ws;

    // collect the contents of the brackets into a stringstream
    readbrackets (in, streambuf);

    // parse based on node name
    hash_t hash (str_hash_fun (node));
    if (hash == VRMLConstants::separator_hash)
    {
	log_debug (log(), "Separator: " << depth_+1);
	VRMLParseObjects::Separator *obj
	    (new VRMLParseObjects::Separator (streambuf, depth_+1));

	children.push_back (obj);
    }

    else if (hash == VRMLConstants::material_hash)
    {
	VRMLParseObjects::Material *obj 
	    (new VRMLParseObjects::Material (streambuf));

	children.push_back (obj);
	log_debug (log(), "Material: " << *obj);
    }

    else if (hash == VRMLConstants::coordinate3_hash)
    {
	VRMLParseObjects::Coordinate3 *obj 
	    (new VRMLParseObjects::Coordinate3 (streambuf));

	children.push_back (obj);
	log_debug (log(), "Coordinate3: " << *obj);
    }

    else if (hash == VRMLConstants::indexed_face_set_hash)
    {
	VRMLParseObjects::IndexedFaceSet *obj 
	    (new VRMLParseObjects::IndexedFaceSet (streambuf));

	children.push_back (obj);
	log_debug (log(), "IndexedFaceSet: " << *obj);
    }

    else
	log_error (log(), "Separator (" << depth_ << ") : unable to parse " << node);
}

std::ostream& operator<< 
(std::ostream& out, const Vector& obj)
{
    out << obj.x << ' ' << obj.y << ' ' << obj.z;
    return out;
}

std::ostream& operator<< 
(std::ostream& out, const Color& obj)
{
    out << obj.red << ' ' << obj.green << ' ' << obj.blue;
    return out;
}

std::ostream& operator<< 
(std::ostream& out, const Coordinate3& obj)
{
    std::vector<MFVec3f>::const_iterator i (obj.points.begin());
    const std::vector<MFVec3f>::const_iterator end (obj.points.end());

    for (; i != end; ++i)
	out << *i << ' ';

    return out;
}

std::ostream& operator<< 
(std::ostream& out, const Material& obj)
{
    out << obj.ambientColor << ' ';
    out << obj.diffuseColor << ' ';
    out << obj.specularColor << ' ';
    out << obj.emissiveColor << ' ';
    out << obj.shininess << ' ';
    out << obj.transparency << ' ';

    return out;
}

std::ostream& operator<< 
(std::ostream& out, const IndexedFaceSet& obj)
{
    std::vector<MFLong>::const_iterator i;
    std::vector<MFLong>::const_iterator end;

    i = obj.coordIndicies.begin();
    end = obj.coordIndicies.end();
    for (; i != end; ++i)
	out << *i << ' ';

    i = obj.materialIndicies.begin();
    end = obj.materialIndicies.end();
    for (; i != end; ++i)
	out << *i << ' ';

    i = obj.normalIndicies.begin();
    end = obj.normalIndicies.end();
    for (; i != end; ++i)
	out << *i << ' ';

    i = obj.textureCoordIndicies.begin();
    end = obj.textureCoordIndicies.end();
    for (; i != end; ++i)
	out << *i << ' ';

    return out;
}

std::istream& operator>> 
(std::istream& in, Vector& obj)
{
    Vector tmp (in);
    obj = tmp;

    return in;
}

std::istream& operator>> 
(std::istream& in, Color& obj)
{
    Color tmp (in);
    obj = tmp;

    return in;
}

std::istream& operator>> 
(std::istream& in, Material& obj)
{
    Material tmp (in);
    obj = tmp;

    return in;
}

std::istream& operator>> 
(std::istream& in, Coordinate3& obj)
{
    Coordinate3 tmp (in);
    obj = tmp;

    return in;
}

std::istream& operator>> 
(std::istream& in, IndexedFaceSet& obj)
{
    IndexedFaceSet tmp (in);
    obj = tmp;

    return in;
}

}}
