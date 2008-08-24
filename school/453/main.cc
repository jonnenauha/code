#include "gfx.hh"

int	main()
{
	float	val1[3]	= { 1.0, 1.0, 1.0 };
	float	val2[3]	= { 1.5, 0.0, 0.0 };
	float	val3[3]	= { 2.0, 3.0, 4.0 };
	
	gfx::Vertex	a( val1 );
	gfx::Point	b( val2 );
	
	(a+b).print();
	
	gfx::VertexList	vl;
	gfx::FaceList	fl;
	
	gfx::Mesh	m( &vl, &fl );
	
	return	0;
}
