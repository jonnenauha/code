//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: Approximates a random wheel, where the relative weights
//		of a integer vector are a percentage of the wheel. Choosing an
//		element on the wheel has a probability equal to its percentage.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------
#ifndef WHEEL_H
#define WHEEL_H


#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>

#include "random.h"


using namespace std;


//================================================================================
//
class	RandomWheel
{
//--------------------------------------------------------------------------------
//
	public:
		RandomWheel(){}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Find the percentage each value contributes to the whole and allot it "space"
// on the wheel proportially. Note since we are seeking to minimize functions, we
// subtract that percentage from the whole.
		
		void	setValues( vector<int>* value )
		{
			vector<int>::iterator val_iter = value->begin();
			
			int	i,j;			
			float	total	= accumulate( value->begin(), value->end(), 0 );
			int	percent	= 0;
			
			_wheel.clear();
			
			for( i=0; i < value->size(); i++ )
			{
				if( val_iter[i] >= 0 )
				{
					percent = 100.0 - ( val_iter[i] * 100.0 ) / total;
					
					for( j=0; j < percent; j++ )
					{
						_wheel.push_back( i );
					}
				}
				else
				{}	// value is invalid
			}
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Pick a random value on the wheel.
		
		int	spin()
		{
			return _wheel[ random_long( 0, _wheel.size() ) ];
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void	print()
		{
			for( int i=0; i < _wheel.size(); i++ )
			{
				cout << i << "th spoke = " << _wheel[i] << endl;
			}
		}
		

//--------------------------------------------------------------------------------
// The wheel space.
	
	private:
		vector<int>	_wheel;
};


#endif
