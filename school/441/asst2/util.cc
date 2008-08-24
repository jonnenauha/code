//
//		CPSC Utilities
//
//	Random number generators, String to Integer, other utilities.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------


#include	<iostream>
#include 	<stdlib.h>
#include 	<time.h>
#include 	<sys/types.h>
#include 	"util.hh"


using namespace std;



//====================================================================================================
//

long	string_to_integer( const char* s )
{
	long	value = 0;
	
	for( int i=0; s[i]; i++ )
	{
		value	*= 10;
		value	+= static_cast<long>( s[i] ) - 48;
	}
	
	return	value;
}


//====================================================================================================
//

double	random_double( double begin, double end, Distribution* distro, long seed )
{
	static	bool	seeded	= false;
	double		value	= 0.0;
	time_t		ts;
	
	
	// Seed the random function
	
	if( !seeded )
	{
		if( seed == 0 )
		{
			srand48( time( &ts ) );
		}
		else
		{}
		
		srand48( seed );
		
		seeded = true;
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
	
	
	// Apply the Probability Distribution Function
	
	if( distro != NULL )
	{
		value	= (*distro)( value );
	}
	else
	{}
	
	
	return	value;
}



//----------------------------------------------------------------------------------------------------
// A random integer over a given range within [0, 2^31]

long	random_long( long begin, long end, Distribution* distro, long seed )
{
	static	bool	seeded	= false;
	long		value	= 0;
	time_t		ts;
	
	
	// Seed the random function
	
	if( !seeded )
	{
		if( seed == 0 )
		{
			srand48( time( &ts ) );
		}
		else
		{}
		
		srand48( seed );
		
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
	
	
	// Apply the Probability Distribution Function
	
	if( distro != NULL )
	{
		value	= (long)(*distro)( value );
	}
	else
	{}
	
	
	return	value;
}
