//************************************************************************************************************************
//
//			CPSC 441 - Assignment 4
//
//	Classes and functions that have general utility.
//
//							Copyright:	Ryan McDougall -- 2003
//
//************************************************************************************************************************



//========================================================================================================================
// Includes and Defines
	
	#ifndef		__INCLUDE_CC	
	#include	"utility.hh"
	#endif

	using namespace std;



//========================================================================================================================
// This includes useful functionality for general programming issues, including memory managment, and random numbers.

namespace	Utility
{
	
//________________________________________________________________________________________________________________________
// Converts a C++ string to int. TODO: use more std C functions.
	
	int		string_to_integer( const string& s )
	{
		int	result	= 0;
		
		for( int i=0; s[i]; i++ )
		{
			result	*= 10;
			
			if(( s[i] >= _ASCII_NUM_BEGIN )&&( s[i] < _ASCII_NUM_END ))
			{
				result	+= static_cast<int>( s[i] -_ASCII_NUM_BEGIN );
			}
			else
			{
				break;
			}
		}
		
		return	result;
	}


//________________________________________________________________________________________________________________________
// Converts an int to a C++ string. TODO: As above.

	string	integer_to_string( const int v )
	{
		int	value		= v;
		bool	negative	= false;
		
		string	result;
		

		if( value < 0 )
		{
			negative = true;
			value 	*= -1;
		}
		else
		{}
		
		while( value > 0 )
		{
			result	= static_cast<char>( (value%10)+_ASCII_NUM_BEGIN ) + result;
			
			value	/= 10;
		}
		
		if( negative )
		{
			result	= '-' + result;
		}
		else
		{}
		
		return	result;
	}



//________________________________________________________________________________________________________________________
// A bounds and type safe replacement for C style buffers.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default Constructor creates a buffer of size Types.

	template < typename Type >
		buffer<Type>::buffer( int size )
		 : _size( size )
	{
		_data	= new Type[ _size ];
		set( 0 );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy constructor for pre allocated C style buffers.

	template < typename Type >		
		buffer<Type>::buffer( const Type* data, int size )
		 : _size( size )
	{
		_data	= new Type[ _size ];
		set( 0 );
		
		::memcpy( begin(), data, size * sizeof(Type) );
	}

				
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy constructor.

	template < typename Type >		
		buffer<Type>::buffer( const buffer<Type>& buffer )
		 : _size( buffer.size() )
	{
		_data	= new Type[ _size ];
		set( 0 );
		
		::memcpy( begin(), buffer.begin(), buffer.size() * sizeof(Type) );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Template specialized copy constructor for C string buffers.

	template <>
		buffer<char>::buffer( const char* str )
	{
		_size	= ::strlen( str );
		_data	= new char[ _size ];
		set( 0 );
		
		strncpy( begin(), str, size() );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default destrcutor.

	template < typename Type >
		buffer<Type>::~buffer()
	{
		delete	[] _data;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Mutable access operator. Minimal bounds checking.

	template < typename Type >
	Type&		buffer<Type>::operator[]( int i )
	{
		return	(( i > 0 )&&( i < _size ))? _data[i] : _data[0];
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Constant acces operator. Minimal bounds checking.

	template < typename Type >
	const Type&	buffer<Type>::operator[]( int i )			const
	{
		return	(( i > 0 )&&( i < _size ))? _data[i] : _data[0];
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Equality operator.

	template < typename Type >
	bool		buffer<Type>::operator==( const buffer<Type>& rhs )	const
	{
		return	( ::memcmp( rhs.begin(), begin(), min( size(), rhs.size() ) * sizeof(Type) ) == 0 );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Concatination operator.

	template < typename Type >
	buffer<Type>&	buffer<Type>::operator+( const buffer& rhs )
	{
		buffer<Type>*	new_buffer	= new buffer<Type>( size() + rhs.size() );
		
		::memcpy( new_buffer.begin(), begin(), size() * sizeof(Type) );
		::memcpy( new_buffer.begin() + size(), rhs.begin(), rhs.size() * sizeof(Type) );
		
		return	*new_buffer;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Assignment operator.

	template < typename Type >
	buffer<Type>&	buffer<Type>::operator=( const buffer& rhs )
	{
		delete	[] _data;
				
		_size	= rhs.size();
		_data	= new Type[ _size ];
		
		::memcpy( begin(), rhs.begin(), rhs.size() * sizeof(Type) );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Size accessor. Note _size is the number of Types, not bytes.

	template < typename Type >
	int		buffer<Type>::size()		const
	{
		return	_size;
	}

					
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Address ( of bytes ) and Size ( in number of bytes ).

	template < typename Type >
	size_t		buffer<Type>::byte_size()	const
	{
		return	_size * sizeof( Type );
	}


	template < typename Type >
	void*		buffer<Type>::byte_data()
	{
		return	static_cast<void*>( _data );
	}
				

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Address of beginning of buffer.

	template < typename Type >
	Type*		buffer<Type>::begin()
	{
		return	_data;
	}

	template < typename Type >
	const Type*	buffer<Type>::begin()		const
	{
		return	_data;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Address of one Type past the end of buffer.

	template < typename Type >
	Type*	buffer<Type>::end()
	{
		return	_data + _size;
	}

	template < typename Type >
	const Type*	buffer<Type>::end()		const
	{
		return	_data + _size;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Sets the buffer to a specific value.

	template < typename Type >
	void		buffer<Type>::set( const Type& value )
	{
		::memset( begin(), value, size() * sizeof(Type) );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Resizes the buffer.

	template < typename Type >
	void		buffer<Type>::resize( int size )
	{
		Type*	data	= new Type[ size ];
		set( 0 );
		
		::memcpy( data, begin(), min(size,size()) * sizeof(Type) );
				
		delete	[] _data;
		
		_size	= size;
		_data	= data;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Print the buffer contents in a space delimited fashion.

	template < typename Type >
	void		buffer<Type>::print()
	{
		for( int i=0; i < _size; i++ )
		{
			cout << _data[i] << " ";
		}
		
		cout << endl;
	}



//________________________________________________________________________________________________________________________
// Returns a random double in the range [begin, end). TODO: Plugin distribution functions.

	double	random_double( double begin, double end, long seed )
	{
		static	bool	seeded	= false;
		double		value	= 0.0;
		time_t		ts;
	
	
		// Seed the random function
	
		if( !seeded )
		{
			if( seed == 0 )
			{
				unsigned short	seed[3];
			
				seed[0]	= (short)( time( &ts ) );
				seed[1]	= (short)( time( &ts ) * ( begin + end ) );
				seed[2]	= (short)( time( &ts ) * seed[0] * seed[1] );
			
				seed48( seed );
			}
			else
			{
				srand48( seed );
			}
		}
		else
		{}
	
	
		// Generate a uniformly random number within the range
	
		if( begin < end )
		{
			value	= begin + ( drand48() * ( end - begin ));
		}
		else if( end < begin )
		{	
			value	= end + ( drand48() * ( begin - end ));
		}
		else
		{
			value	= begin;
		}
	
		return	value;
	}



//------------------------------------------------------------------------------------------------------------------------
// A random integer over a given range [begin, end) within [0, 2^31].

	long	random_long( long begin, long end, long seed )
	{
		static	bool	seeded	= false;
		long		value	= 0;
		time_t		ts;
	
	
		// Seed the random function
	
		if( !seeded )
		{
			if( seed == 0 )
			{
				unsigned short	seed[3];
			
				seed[0]	= (short)( time( &ts ) );
				seed[1]	= (short)( time( &ts ) * ( begin + end ) );
				seed[2]	= (short)( time( &ts ) * seed[0] * seed[1] );
			
				seed48( seed );
			}
			else
			{
				srand48( seed );
			}
		
			seeded = true;
		}
		else
		{}
	
	
		// Generate a uniformly random number within the range
	
		if( begin < end )
		{
			value	= begin + ( lrand48() % ( end - begin ));
		}
		else if( end < begin )
		{
			value	= end + ( lrand48() % ( begin - end ));
		}
		else
		{
			value	= begin;
		}
	
		return	value;
	}

}



//========================================================================================================================
// This includes constructs for handling various generic error conditions.

namespace	Error
{

//________________________________________________________________________________________________________________________
// For automatically handling a broad range of every day error conditions. TODO: ErrorManaged Objects.

		manager::manager()
	{}
			

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
			
		manager::manager( int argc, char** argv )
	{
		_hostname	= argv[0];
	}
			

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	void	manager::operator()( const string msg, err_handler handle, int err )
	{
		cerr << _hostname << ": " << msg << flush << "[ " << strerror( errno ) << " ]" << endl;
				
		if( handle )
		{
			(*handle)( err );
		}
		else
		{}
	}
			

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	void	manager::operator()( const char* msg, err_handler handle, int err )
	{
		cout << "WTF" << endl << flush;
		
		cout << _hostname << ": " << msg << flush << "[ " << strerror( errno ) << " ]" << endl;
				
		if( handle )
		{
			(*handle)( err );
		}
		else
		{}
	}
			

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Automatically logs error messages to a user supplied file, or the system logger.
			
	void	manager::log()
	{}



//________________________________________________________________________________________________________________________
// Basic error printing to cerr, with errno to string translation.

	void	printer( string msg )
	{
		cerr << msg << flush << " [ " << strerror( errno ) << " ]" << endl;
		
		errno	= 0;
	}
			

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Basic error printing to cerr for C strings, with errno to string translation.

	void	printer( const char* msg )
	{
		cerr << msg << flush << " [ " << strerror( errno ) << " ]" << endl;
		
		errno	= 0;
	}


}



//========================================================================================================================
// This includes high level constructs and abstractions for low level file access.

namespace	File
{
//________________________________________________________________________________________________________________________
// Wrapper for file descriptors. TODO: Inheritable to Sockets, etc.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default file constructor creates no file.

		descriptor::descriptor()
		 : _file_desc( -1 )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Creates a system file descriptor pointing to the path.
	
		descriptor::descriptor( const char* path, int oflags, mode_t mode )
		 : _file_desc( -1 ), _path( path )
	{
		_file_desc	= ::open( path, oflags, mode );
		
		if( _file_desc < 0 )
		{
			Error::printer( "Failed to open " + _path );
		}
		else
		{}
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default desctructor.
		
		descriptor::~descriptor()
	{
		::close( _file_desc );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Open a new file. TODO: Semantics of opening while another file exists.

	void	descriptor::open( const char* path, int oflags, mode_t mode )
	{
		if( _file_desc < 0 )
		{
			_path		= path;
			_file_desc	= ::open( path, oflags, mode );
					
			if( _file_desc < 0 )
			{
				Error::printer( "Failed to open " + _path );
			}
			else
			{}
		}
		else if( _path != path )
		{
			Error::printer( "Cannot open a new file over " + _path );
		}
		else
		{}
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Low level read.

	template < typename Type >
	size_t	descriptor::read( Utility::buffer<Type>& buffer )
	{
		size_t	bytes_read = read( _file_desc, buffer.byte_data(), buffer.byte_size()  );
			
		if( bytes_read > 0 )
		{}
		else
		{
			Error::printer( "Unable to read" );
		}
		
		return	bytes_read;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Low level write.

	template < typename Type >
	size_t	descriptor::write( Utility::buffer<Type>& buffer )
	{
		size_t	bytes_writ = write( _file_desc, buffer.byte_data(), buffer.byte_size()  );
				
		if( bytes_writ > 0 )
		{}
		else
		{
			Error::printer( "Unable to read" );
		}
				
		return	bytes_writ;
	}
}
