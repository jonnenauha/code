//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: This module encapsulates only features necessary to
//		an evolving group: a population and the means to cull and 
//		reproduce that population.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------
#ifndef POPULATION_H
#define POPULATION_H


#include <iostream>

#include "main.h"
#include "genome.h"
#include "wheel.h"


const float	TOPBREED	= 0.15;
const float	BTMBREED	= 0.05;


//================================================================================
//
class Population
{
//--------------------------------------------------------------------------------
//
	public:
		Population( int population_size, int number_parm, int size_parm )
		{
			// Greate a group of genomes
			_population	= new vector<Genome*>( population_size );
			_pop_iter	= _population->begin();
			
			for( int i=0; i < _population->size(); i++ )
			{
				_pop_iter[i] = new Genome( number_parm * size_parm );
			}
			
			
			// Keep a seperate fitness vector for convenience
			_fitness_values	= new vector<int>( population_size );
			_fit_iter	= _fitness_values->begin();
			fill( _fitness_values->begin(), _fitness_values->end(), GA_INFINITY );
			
			
			// Allow user specified fitness function
			_fitness_function = NULL;
		}
		
		
		~Population()
		{
			for( int i=0; i < _population->size(); i++ )
			{
				delete _pop_iter[i];
			}
			
			delete _fitness_values;			
			delete _population;
		}
	
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// This controls the life cycle of the population.

		void	nextGeneration()
		{
			evaluateFitness();
			sortFitness();
			
			cull();
			reproduce();
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Find each individual's fitness.

		void	evaluateFitness()
		{
			int fit	= 0;		
			
			for( int i=0; i < _population->size(); i++ )
			{
				fit = _fitness_function( _pop_iter[i] );
				
				_pop_iter[i]->fitness( fit );
				_fit_iter[i] = fit;
			}
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Sort by fitness for later culling.
		
		void	sortFitness()
		{
			sort( _population->begin(), _population->end(), _fitness_compare );
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Reproduce sexually to replace culled genomes.

		void	reproduce()
		{
			int x, y;
			
			_wheel.setValues( _fitness_values );
			
			for( int i=0; i < _population->size(); i++ )
			{
				if( _pop_iter[i]->alive() )
				{}
				else
				{	
					x = _wheel.spin();
					y = _wheel.spin();
					
					_pop_iter[i]->spawnedFrom( _pop_iter[x], _pop_iter[y] );
					_pop_iter[i]->alive( true );
				}
				
			}
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Remove unfit individuals from the breeding stock.
		
		void	cull()
		{
			int r = _population->size() * TOPBREED;
			int s =  _population->size() - ( _population->size() * BTMBREED );
			
			for( int i = r; i < s; i++ )
			{
				_pop_iter[i]->alive( false );
				_fit_iter[i] = GA_INFINITY;
			}
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Allow user to set the fitness function.

		void	setFitnessFunction( FitnessFunction function )
		{	_fitness_function = function;		}

	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void	print()
		{
			for( int i=0; i < _population->size(); i++ )
			{
				cout << i << ": ";
				
				if( _pop_iter[i]->alive() )
				{	cout << "Alive\t";	}
				else
				{	cout << "Dead \t";	}
									
				_pop_iter[i]->print();
			}
		}
		
//--------------------------------------------------------------------------------
// The population's state.

	private:	
		vector<Genome*>*		_population;
		vector<Genome*>::iterator	_pop_iter;
		
		vector<int>*			_fitness_values;
		vector<int>::iterator		_fit_iter;
		
		FitnessFunction			_fitness_function;
		
		GenomeComparator		_fitness_compare;
		RandomWheel			_wheel;
};

#endif
