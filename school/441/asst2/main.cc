//****************************************************************************************************
//
//			CPSC 441 - Assignment 2
//
//		Simulate DataLink Layer Medium contention.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************



//====================================================================================================
// Includes and Defines

#include	<iostream>
#include 	<vector>
#include	"util.hh"

using namespace std;



//====================================================================================================
// Global Variables

const int			BZN_NULL		= 0;		// Bozon States
const int			BZN_SLEEP		= 1;
const int			BZN_YODEL		= 2;
const int			BZN_SCREECH		= 3;

ExponentialDistribution*	GLB_SLEEP_DISTR		= NULL;		// Exponential PDF Functors
ExponentialDistribution*	GLB_YODEL_DISTR		= NULL;

const bool			FIXED_YODEL		= false;	// Simulation Parameters

int				GLB_POPN_SIZE		= 0;		
int				GLB_TIME_LEN		= 0;
int				GLB_TIME_TICKS		= 0;
int				GLB_MEAN_SLEEP		= 0;
int				GLB_MEAN_YODEL		= 0;

int				GLB_NUM_SCREECHES	= 0;		// Simulation results
int				GLB_NUM_YODELS		= 0;

int				GLB_MEDIUM_ACCESS	= 0;		// Medium Access Counter



//====================================================================================================
// Object represents a Bozon.

class	Bozon
{
//----------------------------------------------------------------------------------------------------
//
	public:

			Bozon()
		{
			_state		= BZN_SLEEP;
			_yodel_time	= 0;
			_sleep_time	= 
				static_cast<int>( random_double( 0.0, 1.0, GLB_SLEEP_DISTR ) );
		}
		
		
		bool	isSleeping()
		{ 
			return	(( _state == BZN_SLEEP )&&( GLB_TIME_TICKS < _sleep_time ));
		}
		
		bool	isYodeling()
		{
			return	(( _state == BZN_YODEL )&&( GLB_TIME_TICKS < _yodel_time ));
		}
		
		bool	isIdle()
		{
			return	!( isSleeping() || isYodeling() );
		}
		
		bool	isSleepy()
		{
			return	(( _state == BZN_YODEL ) && !isYodeling() );
		}
		
		
		void	sleep( int time )
		{
			GLB_MEDIUM_ACCESS --;	// Release Medium
			
			_state		= BZN_SLEEP;
			_sleep_time	= GLB_TIME_TICKS + time;
		}
		
		void	yodel( int time )
		{
			GLB_MEDIUM_ACCESS ++;	// Aquire Medium
			
			_state		= BZN_YODEL;
			_yodel_time	= GLB_TIME_TICKS + time;
		}
		

//----------------------------------------------------------------------------------------------------
//
	private:
	
		int	_sleep_time;
		int	_yodel_time;
		int	_state;
};



//====================================================================================================
// Main Simulation Logic

int	main( int argc, char** argv )
{
	// Parse comand line parameters.
	
	if( argc != 5 )
	{
		cout	<< "Usage: ./bozon [POPULATION_SIZE] [SIMULATION_LENGTH] "
			<< "[MEAN_SLEEP_TIME] [MEAN_YODEL_TIME]" << endl;
	}
	else
	{
		double 		time = 0;
		
		GLB_POPN_SIZE	= string_to_integer( argv[1] );
		GLB_TIME_LEN	= string_to_integer( argv[2] );
		GLB_MEAN_SLEEP	= string_to_integer( argv[3] );
		GLB_MEAN_YODEL	= string_to_integer( argv[4] );
		
		GLB_SLEEP_DISTR	= new ExponentialDistribution( GLB_MEAN_SLEEP );
		GLB_YODEL_DISTR	= new ExponentialDistribution( GLB_MEAN_YODEL );
		
		vector<Bozon>	population( GLB_POPN_SIZE );

		
		
		// Run Simulation.
		
		while( GLB_TIME_TICKS < GLB_TIME_LEN )
		{
		
			// Affect the populations' state.
			
			for( int i=0; i < population.size(); i++ )
			{
			
				if( population[i].isIdle() )
				{
				
					if( population[i].isSleepy() )
					{
						time	= random_double( 0.0, 1.0, GLB_SLEEP_DISTR );
									
						population[i].sleep( static_cast<int>( time ) );
					}
					else
					{
						if( FIXED_YODEL )
						{
							time	= 10;
						}
						else
						{
							time	= random_double( 0.0, 1.0, GLB_YODEL_DISTR );
						}
									
						population[i].yodel( static_cast<int>( time ) );
					}
				}
				else
				{
					// Everyone is busy.
				}
			}
			
			
			
			// Check for collisions.
			
			if( GLB_MEDIUM_ACCESS == 1 )
			{
				GLB_NUM_YODELS ++;
			}
			else if( GLB_MEDIUM_ACCESS > 1 )
			{
				GLB_NUM_SCREECHES ++;
			}
			else
			{
				// Everybody is sleeping.
			}
			
			
			
			// Increment the time.		
	
			GLB_TIME_TICKS ++;
		}


		// Print stats
		
		//cout 	<< "Number of Yodels: \t" << GLB_NUM_YODELS << endl
		//	<< "Number of Screeches: \t" << GLB_NUM_SCREECHES << endl
		//	<< "Total Attempted: \t" << GLB_NUM_YODELS + GLB_NUM_SCREECHES << endl;
		
		float	melodious	= (float)GLB_NUM_YODELS / (float)GLB_TIME_LEN;
		float	screechy	= (float)GLB_NUM_SCREECHES / (float)GLB_TIME_LEN;
		float	silent		= (float)( GLB_TIME_LEN - GLB_NUM_YODELS - GLB_NUM_SCREECHES )
						/ (float)GLB_TIME_LEN;
		
		cout	<< GLB_POPN_SIZE << "\t" << silent << "\t" << melodious << "\t" << screechy << endl;
	}
	
	return 0;
}
