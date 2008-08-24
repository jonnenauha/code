//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: This module encapsulates only features necessary to
//		an evolving individual: its genetic code, state of life, 
//		fitness to its environment, and the severity of mutation in that
//		evironment. All mutation and reproductions are bit string based.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------
#ifndef GENOME_H
#define GENOME_H


#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "main.h"
#include "random.h"


using namespace std;


const int	CRSPROB 	= 30;
const int	MUTPROB 	= 20;
const float	MUTSEVR 	= 0.30;


//================================================================================
//
class Genome
{
//--------------------------------------------------------------------------------
//
	public:
		Genome( int size )
		{	
			_alive		= true;
			_fitness	= GA_INFINITY;
			
			
			// Create space for the genome code
			_code		= new vector<bool>( size );
			_code_iter	= _code->begin();
			
			
			// Random intital configuration
			for( int i=0; i < _code->size(); i++ )
			{	
				// Random boolean values
				_code_iter[i] = random_long( 0, 2 );
			}
			
			
			// Control the mutation severity
			_mutation_severity = _code->size() * MUTSEVR;
		}
		
			~Genome()
		{
			delete _code;
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Accessors and Mutators

		int	fitness()
		{	return _fitness;	}
		void	fitness( int fitness )
		{	_fitness = fitness;	}
		
		bool	alive()
		{	return _alive;		}
		void	alive( bool alive )
		{	_alive = alive;		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Sexual reproduction.
		
		void	spawnedFrom( Genome* x, Genome* y )
		{
			bool	p = 0;
			int	r = 0;
			int	s = random_long( 0, _code->size() );
			
			while( r < _code->size()-1 )
			{
				if( p )
				{
					copy( x->_code_iter+r, x->_code_iter+s, _code_iter+r );
				}
				else
				{
					copy( y->_code_iter+r, y->_code_iter+s, _code_iter+r );
				}
				
				r = random_long( s, _code->size() );
				s = random_long( r, _code->size() );
				p = ~p;
			}
			
			mutate();
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Genome crossover.

		void	crossover( Genome* x )
		{
			if( random_long( 0, 100 ) < CRSPROB )
			{
				int	r = random_long( 0, _code->size() );
				int	s = random_long( r, _code->size() );
		
				swap_ranges( _code_iter+r, _code_iter+s, x->_code_iter+r );
			}
			else
			{}
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Random mutation.
	
		void	mutate()
		{
			if( random_long( 0, 100 ) < MUTPROB )
			{		
				int	r = random_long( 0, _code->size() );
				int	s = random_long( r, _code->size() );
				
				if( ( s - r ) <= _mutation_severity )
				{
					random_shuffle( _code_iter+r, _code_iter+s );
				}
				else
				{}
			}
			else
			{}
		}
	

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void		setCode( vector<bool>& g )
		{
			copy( g.begin(), g.end(), _code->begin() );
		}
		
		vector<bool>*	getCode()
		{
			return _code;
		}		
				
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		bool	operator<( Genome& g )
		{
			if( _fitness < g._fitness )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void	print()
		{
			for( int i=0; i < _code->size(); i++ )
			{
				cout << _code_iter[i];
			}
			
			cout << " : " << _fitness << endl;
		}

		
//--------------------------------------------------------------------------------	
// The genome's state.

	private:
		vector<bool>*		_code;
		vector<bool>::iterator	_code_iter;
		
		bool			_alive;
		int			_fitness;
		
		int			_mutation_severity;
};



//================================================================================
// This functor is used by the STL to compare the fitness of two Genome pointers, 
// to see which is more fit. It relies on the overloaded less than operator.

class GenomeComparator
{
	public:
		GenomeComparator(){}
		
		bool	operator()( Genome* a, Genome* b )
		{
			return *a < *b;
		}
		
	private:
};


#endif
