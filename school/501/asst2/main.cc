//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: Main control logic. This contains assignment specific
//		elements, such as reading from a file, evaluating a Gaussian
//		equation, and the instance of the fitness function to be used.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <cmath>

#include "population.h"
#include "decoder.h"


using namespace std;


// Global Variables
vector<double>	glb_data;
GenomeDecoder	decoder( NUMPARMS, PARMSIZE );



//--------------------------------------------------------------------------------
// Computes the Gaussian function at x, with constant parameters I, M, S.

double	evaluate_gaussian( double I, double M, double S, double x )
{
	double	D = 2.0 * S * S;
	double	r = 0.0;
	
	if( D >= 0.0 )
	{
		r = I*exp( -1*( x-M )*( x-M )/ D );
	}
	else
	{
		r = 0.0;
		cerr << "Division by zero in \"evaluate_gaussian\"." << endl;
	}
	
	return r;
}


//--------------------------------------------------------------------------------
// Reads input file. Assumes Domain is [1,100)

void	read_file( char* filename, vector<double>& data_buffer )
{
	double		d = 0.0;
	ifstream 	data_file( filename );
	
	data_buffer.reserve( 100 );
	
	if( data_file )
	{
		while( !data_file.eof() )
		{
			data_file >> d;		// Index
			data_file >> d;		// Real stuff
			data_buffer.push_back( d );
		}
	}
	else
	{}
	
	data_file.close();
}


//--------------------------------------------------------------------------------
// Our fitness function. It is supposed to decode the genome bit string into the
// correct parameter set, and compare the sum of Gaussians with the input data.
// However since decoding is incomplete it doesnt work. It is a simple matter of
// time to get a proper mapping between bit strings and real numbers.

int	fit_gaussian_to_data( Genome* g )
{
	// float g1 = evaluate_gaussian( I1, M1, S1, x );
	// float g2 = evaluate_gaussian( I2, M2, S2, x );
	// return  g1 + g2 - glb_data[x];
	
	return random_long( 0, 100 );
}


//--------------------------------------------------------------------------------
//
int	main( int argc, char** argv )
{	
	if( argc == 2 )
	{
		read_file( argv[1], glb_data );
		
		Population 	p( POPSIZE, NUMPARMS, PARMSIZE );
		p.setFitnessFunction( &fit_gaussian_to_data );
		
		for( int i=0; i < NUMGENS; i++ )
		{
			p.nextGeneration();
		}
	}
	else
	{
		cerr << "No file." << endl;
	}

	return 0;
}
