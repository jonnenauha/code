//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: Global constants and other information.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------
#ifndef MAIN_H
#define MAIN_H

#include <vector>

using namespace std;

//--------------------------------------------------------------------------------
// Global constants, useful data types, and functions.

const int	GA_INFINITY	= -1;

const int	NUMGENS		= 100;
const int	POPSIZE		= 100;
const int	NUMPARMS	= 3;
const int	PARMSIZE	= 32;


class Genome;
typedef	int (*FitnessFunction)( Genome* );


double	evaluate_gaussian( double I, double M, double S, double x );
void	read_file( char* filename, vector<double>& data_buffer );
int	fit_gaussian_to_data( Genome* g );


#endif

