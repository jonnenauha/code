//
//		CPSC Utilities
//
//	Random number generators, String to Integer, other utilities.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------

#ifndef __MAIN_HH
#define __MAIN_HH



//====================================================================================================
// Includes and Defines

#include	<cmath>


using namespace std;



//----------------------------------------------------------------------------------------------------
// Interface for a Probability Distribution Function functors.

class	Distribution
{
	public:		
		virtual	double	operator()( double x ) const	= 0;
};


//----------------------------------------------------------------------------------------------------
// Implementation of the Exponential PDF, with parameter Mu = mean.

class	ExponentialDistribution	: public Distribution
{

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
	public:	
			ExponentialDistribution( double Mu )
			: _mean( Mu )
		{}
		
		
		double	operator()( double x ) const
		{
			return	( -1.0 * _mean * log( x ) );
		}
		

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
	private:
		double	_mean;
};



//----------------------------------------------------------------------------------------------------
// Utility Functions

long	random_long( long begin, long end, Distribution* distro = NULL, long seed = 0 );
double	random_double( double begin = 0.0, double end = 1.0, Distribution* distro = NULL, long seed = 0 );

long	string_to_integer( const char* s );


#endif
