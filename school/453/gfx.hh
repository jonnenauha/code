//
//	Classes for easily doing computer geometry.
//
//		Copyright Ryan McDougall -- 2003
//

#ifndef __GFX_H__
#define __GFX_H__

#include	<vector>


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
	// Vector Arithmetic Methods

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
	/*
	template < int Dimension = DIMENSION, typename Type = DEFAULT_TYPE >
	abstract_vector<Dimension,Type>&	
	operator+( Type* lhs, const abstract_vector<Dimension,Type>& rhs )
	{
		abstract_vector<Dimension,Type>	result;
		
		for( int i=0; i < Dimension; i++ )
		{	result[i] = lhs[i] + rhs[i];	}
		
		return	result;	
	}


	template < int Dimension = DIMENSION, typename Type = DEFAULT_TYPE >
	abstract_vector<Dimension,Type>&
	operator-( Type* lhs, const abstract_vector<Dimension,Type>& rhs )
	{
		abstract_vector<Dimension,Type>	result;
		
		for( int i=0; i < Dimension; i++ )
		{	result[i] = lhs[i] - rhs[i];	}
		
		return	result;		
	}

	
	template < int Dimension = DIMENSION, typename Type = DEFAULT_TYPE >
	abstract_vector<Dimension,Type>&
	operator*( Type scalar, const abstract_vector<Dimension,Type>& rhs )
	{
		abstract_vector<Dimension,Type>	result;
    
		for( int i=0; i < Dimension; i++ )
		{	result._p[i] = _p[i] * scalar;	}
  
		return	result;
	}*/
		
	
	
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
	
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>			Point;
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>			Direction;
	typedef	abstract_vector<DIMENSION,DEFAULT_TYPE>			Vertex;
	typedef	std::vector< abstract_vector<DIMENSION,DEFAULT_TYPE> >	VertexList;
	typedef	std::vector<int>					Face;
	typedef	std::vector<Face>					FaceList;
	
	
	
	//==================================================================================================
	// Mesh Class
	
	class	Mesh
	{
		public:

	//--------------------------------------------------------------------------------------------------
	// Constructors and Destructors

			Mesh( const VertexList* vertex_list, const FaceList* face_list )
			{
				_vertex_list	= new VertexList( *vertex_list );
				_face_list	= new FaceList( *face_list );
			}
			
			Mesh( const Mesh& copy )
			{
				_vertex_list	= new VertexList( *(copy._vertex_list) );
				_face_list	= new FaceList( *(copy._face_list) );
			}
			
			virtual
			~Mesh()
			{
				delete	_vertex_list;
				delete	_face_list;
			}
		
		
	//--------------------------------------------------------------------------------------------------
	//
		protected:
			VertexList*	_vertex_list;
			FaceList*	_face_list;
		
		private:
			Mesh(){}
	};

}

#endif // __GFX_H__
