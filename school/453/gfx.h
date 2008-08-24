//
//	Classes for easily doing computer geometry.
//
//		Copyright Miho Nakai, Ryan McDougall -- 2003
//

#ifndef __GFX_H__
#define __GFX_H__

#include	<vector>
#include 	<GL/glut.h>

namespace gfx
{
	typedef	float	DEFAULT_TYPE;	
	const	int	DIMENSION 	= 3;
	enum	axis	{ X, Y, Z, W };
		
	
	
	//==================================================================================================
	// Abstract vector class
	
	template < int Dimension = DIMENSION, typename Type = DEFAULT_TYPE >
	class	abstract_vector
	{
		public:

	//--------------------------------------------------------------------------------------------------
	// Constructors and Destructors
			abstract_vector()
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] = 0.0;		} 
			}
  
			abstract_vector( const Type* copy )
			{				
				for( int i=0; i<Dimension; i++ )
				{	_p[i] = copy[i];	}
			}
  
  
			abstract_vector( const abstract_vector<Dimension,Type>& copy )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] = copy._p[i];	}		
			}
			
			virtual
			~abstract_vector()
			{}


	//--------------------------------------------------------------------------------------------------
	// Element Accessor Methods

			virtual
			Type	
			operator[]( axis i ) const
			{
				return	_p[ i % Dimension ];
			}
  
			virtual
			Type&	
			operator[]( axis i )
			{
				return	_p[ i % Dimension ];
			}


	//--------------------------------------------------------------------------------------------------
	// Scalar Arithmetic Methods

			virtual	
			abstract_vector<Dimension,Type>	
			operator*( const Type scalar ) const
			{
				abstract_vector<Dimension,Type>	result;
    
				for( int i=0; i<Dimension; i++ )
				{	result._p[i] = _p[i] * scalar;	}
    
				return	result;
			}
  
			virtual
			abstract_vector<Dimension,Type>	
			operator/( const Type scalar ) const
			{
				abstract_vector<Dimension,Type>	result;
    
				for( int i=0; i<Dimension; i++ )
				{	result._p[i] = _p[i] / scalar;	}
    
				return	result;
			}
			
			virtual	
			abstract_vector<Dimension,Type>&
			operator*=( const Type scalar )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] *= scalar;	}
    
				return	*this;
			}
  
			virtual
			abstract_vector<Dimension,Type>&
			operator/=( const Type scalar )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] /= scalar;	}
    
				return	*this;
			}
  


	//--------------------------------------------------------------------------------------------------
	// Vector Addition/Subraction Methods

			virtual
			abstract_vector<Dimension,Type>	
			operator+( const abstract_vector<Dimension,Type>& rhs ) const
			{
				abstract_vector<Dimension,Type>	result;
    
				for( int i=0; i<Dimension; i++ )
				{	result._p[i] = _p[i] + rhs._p[i];	}
    
				return	result;
			}
  
			virtual
			abstract_vector<Dimension,Type>	
			operator-( const abstract_vector<Dimension,Type>& rhs ) const
			{
				abstract_vector<Dimension,Type>	result;
   
				for( int i=0; i<Dimension; i++ )
				{	result._p[i] = _p[i] - rhs._p[i];	}
    
				return	result;
			}
  
			virtual		
			abstract_vector<Dimension,Type>	
			operator+( const Type* rhs ) const
			{
				abstract_vector<Dimension,Type>	result;
    
				for( int i=0; i<Dimension; i++ )
				{	result._p[i] = _p[i] + rhs[i];		}
    
				return	result;
			}
  
			virtual
			abstract_vector<Dimension,Type>	
			operator-( const Type* rhs ) const
			{
				abstract_vector<Dimension,Type>	result;
    
				for( int i=0; i<Dimension; i++ )
				{	result._p[i] = _p[i] + rhs[i];	}
    
				return	result;
			}

	//--------------------------------------------------------------------------------------------------
	// Vector Addition/Subtraction with Assignment
			
			virtual	
			abstract_vector<Dimension,Type>&
			operator+=( const abstract_vector<Dimension,Type>& rhs )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] += rhs._p[i];	}
    
				return	*this;
			}
  
			virtual
			abstract_vector<Dimension,Type>&
			operator-=( const abstract_vector<Dimension,Type>& rhs )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] += rhs._p[i];	}
    
				return	*this;
			}
			
			virtual	
			abstract_vector<Dimension,Type>&
			operator+=( const Type* rhs )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] += rhs[i];	}
    
				return	*this;
			}
  
			virtual
			abstract_vector<Dimension,Type>&
			operator-=( const Type* rhs )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] += rhs[i];	}
    
				return	*this;
			}
	
	//--------------------------------------------------------------------------------------------------
	// Vector Multiplication ( dot ) Methods
	
			virtual
			Type
			operator*( const abstract_vector<Dimension,Type>& rhs ) const
			{
				Type result;
				
				for( int i=0; i<Dimension; i++ )
				{	result += _p[i] * rhs._p[i];	}
    
				return	result;
			}

			virtual
			Type
			operator*( const Type* rhs ) const
			{
				Type result;
				
				for( int i=0; i<Dimension; i++ )
				{	result += _p[i] * rhs[i];	}
    
				return	result;
			}
	
  

	//--------------------------------------------------------------------------------------------------
	// Assignment Methods

			virtual
			const abstract_vector<Dimension,Type>&
			operator=( const abstract_vector<Dimension,Type>& rhs )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] = rhs._p[i];	}
    
				return	*this;			
			}
  
			virtual
			const abstract_vector<Dimension,Type>&
			operator=( const Type* rhs )
			{
				for( int i=0; i<Dimension; i++ )
				{	_p[i] = rhs[i];		} 
    
				return	*this;
			}


	//--------------------------------------------------------------------------------------------------
	// Accessor Methods
	
			virtual
			const int
			dimension()	const
			{
				return	Dimension;
			}
			
			virtual
			void
			print() const
			{
				std::cout << "[ ";
				for( int i=0; i<Dimension; i++ )
				{
					std::cout << _p[i] << ' ';
				}
				std::cout << "]";
			}
  


	//--------------------------------------------------------------------------------------------------
	// Protected Members

		protected:
			Type	_p[Dimension];
  
	};
		
	
	
	//==================================================================================================
	// Convinience functions for arithmetic on vector class and raw types
	
	const int CROSS_DIM = 3;
	
	template < typename Type >
	abstract_vector<CROSS_DIM,Type>
	cross( const abstract_vector<CROSS_DIM,Type>& lhs, const abstract_vector<CROSS_DIM,Type>& rhs )
	{
		abstract_vector<CROSS_DIM,Type>	result;
		
		result[X] = lhs[Y] * rhs[Z] - lhs[Z] * rhs[Y];
		result[Y] = lhs[Z] * rhs[X] - lhs[X] * rhs[Z];
		result[Z] = lhs[X] * rhs[Y] - lhs[Y] * rhs[X];
		
		return	result;			
	}
	

	template < int Dimension, typename Type >
	abstract_vector<Dimension,Type>	
	operator+( Type* lhs, const abstract_vector<Dimension,Type>& rhs )
	{
		abstract_vector<Dimension,Type>	result;
		
		for( int i=0; i < Dimension; i++ )
		{	result[i] = lhs[i] + rhs[i];	}
		
		return	result;	
	}


	template < int Dimension, typename Type >
	abstract_vector<Dimension,Type>
	operator-( Type* lhs, const abstract_vector<Dimension,Type>& rhs )
	{
		abstract_vector<Dimension,Type>	result;
		
		for( int i=0; i < Dimension; i++ )
		{	result[i] = lhs[i] - rhs[i];	}
		
		return	result;		
	}

	
	template < int Dimension, typename Type >
	abstract_vector<Dimension,Type>
	operator*( Type scalar, const abstract_vector<Dimension,Type>& rhs )
	{
		abstract_vector<Dimension,Type>	result;
    
		for( int i=0; i < Dimension; i++ )
		{	result._p[i] = _p[i] * scalar;	}
  
		return	result;
	}
		
	
	
	//==================================================================================================
	// Affine Point Class
	
	template <int Dimension = DIMENSION, typename Type = DEFAULT_TYPE>
	class	affine_point	: public abstract_vector<Dimension,Type>
	{
		public:

	//--------------------------------------------------------------------------------------------------
	// Constructors and Destructors
	
			affine_point()
			 : _dim( Dimension+1 ), abstract_vector<Dimension+1,Type>()
			{
				_p[ _dim-1 ] = 1;
			}
  
			affine_point( const Type* copy )
			 : _dim( Dimension+1 ), abstract_vector<Dimension+1,Type>( copy )
			{
				_p[ _dim-1 ] = 1;
			}
  
  
			affine_point( const abstract_vector<Dimension,Type>& copy )
			 : _dim( Dimension+1 ), abstract_vector<Dimension+1,Type>( copy )
			{
				_p[ _dim-1 ] = 1;
			}
			
			virtual
			~affine_point()
			{}

	//--------------------------------------------------------------------------------------------------
	// Protected Memebers
	
		protected:
			const int	_dim;
	};
	
	
	
	//==================================================================================================
	// Affine Direction Class
	
	template <int Dimension = DIMENSION, typename Type = DEFAULT_TYPE>
	class	affine_direction	: public abstract_vector<Dimension,Type>
	{
		public:

	//--------------------------------------------------------------------------------------------------
	// Constructors and Destructors
	
			affine_direction()
			 : _dim( Dimension+1 ), abstract_vector<Dimension+1,Type>()
			{
				_p[ _dim-1 ] = 0;
			}
  
			affine_direction( const Type* copy )
			 : _dim( Dimension+1 ), abstract_vector<Dimension+1,Type>( copy )
			{
				_p[ _dim-1 ] = 0;
			}
  
  
			affine_direction( const abstract_vector<Dimension,Type>& copy ) 
		         : _dim( Dimension+1 ), abstract_vector<Dimension+1,Type>( copy )
			{
				_p[ _dim-1 ] = 0;
			}
			
			virtual
			~affine_direction()
			{}

	//--------------------------------------------------------------------------------------------------
	// Protected Memebers
	
		protected:
			const int	_dim;
	};



	//==================================================================================================
	// Line or Ray class ( infinite length )

	template <int Dimension = DIMENSION, typename Type = DEFAULT_TYPE>
	class	abstract_line
	{
		public:

	//--------------------------------------------------------------------------------------------------
	// Constructors and Destructors
	
			abstract_line()
			 : _point(), _direction()
			{}

			abstract_line( const abstract_line<Dimension,Type>& copy )
			 : _point( copy._point ), _direction( copy._direction )
			{}
			
			virtual
			~abstract_line()
			{}

	//--------------------------------------------------------------------------------------------------
	//			
		protected:
			affine_point<Dimension,Type>		_point;
			affine_direction<Dimension,Type>	_direction;
	};
	
	
	
	//==================================================================================================
	// Line Segment class ( finite length )
	
	typedef	DEFAULT_TYPE	LengthType;

	template <int Dimension = DIMENSION, typename Type = DEFAULT_TYPE>
	class	line_segment	: public abstract_line<Dimension,Type>
	{
		public:

	//--------------------------------------------------------------------------------------------------
	// Constructors and Destructors
	
			line_segment()
			 : abstract_line<Dimension,Type>(), _length( 0.0 )
			{}

			line_segment( const line_segment<Dimension,Type>& copy )
			 : abstract_line<Dimension,Type>( copy ), _length( 0.0 )
			{}
			
			virtual
			~line_segment()
			{}

	//--------------------------------------------------------------------------------------------------
	// Length accessor / mutator
	
			LengthType
			length()
			{
				return	_length;
			}


	//--------------------------------------------------------------------------------------------------
	//			
		protected:
			LengthType	_length;
	};
	
		
	
	//==================================================================================================
	// Type defines for using above classes
	
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>		Point;
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>		Direction;
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>		Vertex;  
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>		Normal;
	typedef	std::vector<Vertex>				VertexList;
	typedef std::vector<Normal> 				NormalList; 
	typedef	std::vector<GLint>				Face;
	typedef	std::vector<Face>				FaceList;
	
	
	//==================================================================================================
	// Functions for this assignment
	
	bool	read_obj_file( const char* filename, VertexList* vlist, FaceList* flist );
	

	
	//==================================================================================================
	// Mesh Class ( normal i corresponds to face i )
	
	class	Mesh
	{
		public:
	 	//--------------------------------------------------------------------------------------------------
		// Constructors and Destructors

			Mesh( const VertexList* vertex_list, const FaceList* face_list )
				{
					_vertex_list	= new VertexList( *vertex_list );
					_face_list	= new FaceList( *face_list );
					_normal_list	= new NormalList();
				
					for( int i=0; i<_face_list->size(); i++ )
						{				
							Face face	= (*_face_list)[i];	// face has its vertex info ( index of the vertex )
							Normal normal	= cross( (*_vertex_list)[face[0]] - (*_vertex_list)[face[1]], 
										(*_vertex_list)[face[2]]-(*vertex_list)[face[1]] );
	
							_normal_list->push_back( normal ); // Store the normal in the same order as faceList
						}	
				
				}
			Mesh( const Mesh& copy )
				{
					_vertex_list	= new VertexList( *(copy._vertex_list) );
					_face_list	= new FaceList( *(copy._face_list) );
					_normal_list	= new NormalList( *(copy._normal_list) );
					
				}

			
			virtual
			~Mesh()
			{
				//delete	_vertex_list;
				//delete	_face_list; 
				//delete 	_normal_list;			 
			}
			
			void draw();

	//--------------------------------------------------------------------------------------------------
	//
		protected:
			VertexList*	_vertex_list;
			FaceList*	_face_list;
			NormalList*	_normal_list;
		
		private:
			Mesh(){}
	};
}


#endif // __GFX_H__
