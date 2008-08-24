//************************************************************************************************************************
//
//			CPSC 441 - Assignment 4
//
//							Copyright:	Ryan McDougall -- 2003
//
//************************************************************************************************************************

#ifndef __UTIL_HH
#define __UTIL_HH



//========================================================================================================================
//

	#include	<iostream>
	#include	<string>
	#include	<algorithm>

	#include	<unistd.h>
	#include	<string.h>
	#include	<fcntl.h>
	#include	<sys/types.h>
	#include	<errno.h>
	
	using namespace std;



//========================================================================================================================
//
namespace	Utility
{

	const int	_ASCII_NUM_BEGIN	= 48;
	const int	_ASCII_NUM_END		= 10+_ASCII_NUM_BEGIN;
	
//________________________________________________________________________________________________________________________
//
	
	int		string_to_integer( const string& s );
	string		integer_to_string( const int v );



//________________________________________________________________________________________________________________________
//

	template < typename Type >
	class	buffer
	{
		public:
				buffer( int size );
				buffer( const char* str );
				buffer( const Type* data, int size );
				buffer( const buffer& buffer );
			
			virtual	~buffer();
			

			Type&		operator[]( int i );
			const Type&	operator[]( int i )		const;
			
			bool	operator==( const buffer<Type>& rhs )	const;
			
			buffer&	operator+( const buffer& rhs );
			buffer&	operator=( const buffer& rhs );

			int		size()		const;

			size_t		byte_size()	const;
			void*		byte_data();

			Type*		begin();
			const Type*	begin()		const;

			Type*		end();
			const Type*	end()		const;

			void		set( const Type& value );
			void		resize( int size );
			
			void		print();
			
			
		protected:

			int		_size;
			Type*		_data;

				buffer();
	};



//________________________________________________________________________________________________________________________
//

	double	random_double( double begin, double end, long seed = 0 );
	long	random_long( long begin, long end, long seed = 0 );
}



//========================================================================================================================
//
namespace	Error
{

//________________________________________________________________________________________________________________________
// Error Handling Defines.

	typedef	void (*err_handler)(int);


//________________________________________________________________________________________________________________________
// Create Exception classes and integrate them into the manager.



//________________________________________________________________________________________________________________________
//
	class	manager
	{
		public:
				manager();
				manager( int argc, char** argv );
				
				
			void	operator()( const string msg, err_handler handle = NULL, int err = 0 );
			void	operator()( const char* msg, err_handler handle = NULL, int err = 0 );
			void	log();
			
			
		private:
			char*	_hostname;

	};



//________________________________________________________________________________________________________________________
//

	void	printer( const char* msg );
	void	printer( string& msg );

}



//========================================================================================================================
//
namespace	File
{

//________________________________________________________________________________________________________________________
//
	class	descriptor
	{
		public:
				descriptor();
				descriptor( const char* path, int oflags, mode_t mode = 0 );
				
			virtual	~descriptor();
			

			void		open( const char* path, int oflags, mode_t mode = 0 );
			
			template < typename Type >
			size_t		read( Utility::buffer<Type>& buffer );
			
			template < typename Type >
			size_t		write( Utility::buffer<Type>& buffer );


		private:
			int		_file_desc;
			string		_path;
	};
}



//========================================================================================================================
// Template hack.

#ifdef		__INCLUDE_CC
#include	"utility.cc"
#endif


#endif
