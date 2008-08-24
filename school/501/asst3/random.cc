//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: Utility functions for generating good quality
//		random numbers, in both integer and floating point, over a 
//		specific range, where all ranges are [start, end). Also the user
//		can give a custom seed in the form a long type argument.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------


#include <iostream>
#include <cstdlib>

#include <ctime>
#include <sys/types.h>


using namespace std;


//--------------------------------------------------------------------------------
// A random floating point number over a given range.

double	random_double( double start, double end, long seed = 0 )
{
	static bool	seeded = false;
	time_t		ts;
	double		value = 0.0;
	
	if( seed != 0 )
	{
		srand48( seed );
		seeded = true;
		
		// cerr << "Info: random was seeded." << endl;		
	}
	else
	{
		if( seeded == false )
		{
			srand48( time( &ts ) );
			seeded = true;
		
			// cerr << "Info: random was seeded." << endl;
		}
		else
		{
			// Already seeded.
		}
	}
	
	if( ( end - start ) > 0 )
	{
		value = start + ( drand48() * ( end - start ) );
	}
	else if( ( end - start ) < 0 )
	{
		value = end + ( drand48() * ( start - end ) );
	}
	else
	{
		value = start;
	}
	
	return value;
}


//--------------------------------------------------------------------------------
// A randome floating point number over the range [0, 1]

double	random_double( long seed = 0 )
{
	static bool	seeded = false;
	time_t		ts;
	
	if( seed != 0 )
	{
		srand48( seed );
		seeded = true;
		
		// cerr << "Info: random was seeded." << endl;		
	}
	else
	{
		if( seeded == false )
		{
			srand48( time( &ts ) );
			seeded = true;
		
			// cerr << "Info: random was seeded." << endl;
		}
		else
		{
			// Already seeded.
		}
	}
	
	return drand48();
}


//--------------------------------------------------------------------------------
// A random integer over a given range within [0, 2^31]

long	random_long( long start, long end, long seed = 0 )
{
	static bool	seeded = false;
	time_t		ts;
	long		value = 0;	
	
	if( seed != 0 )
	{
		srand48( seed );
		seeded = true;
		
		// cerr << "Info: random was seeded." << endl;		
	}
	else
	{
		if( seeded == false )
		{
			srand48( time( &ts ) );
			seeded = true;
		
			// cerr << "Info: random was seeded." << endl;
		}
		else
		{
			// Already seeded.
		}
	}
	
	if( ( end - start ) > 0 )
	{
		value = start + ( lrand48() % ( end - start ) );
	}
	else if( ( end - start ) < 0 )
	{
		value = end + ( lrand48() % ( start - end ) );
	}
	else
	{
		value = start;
	}
	
	return value;
}
