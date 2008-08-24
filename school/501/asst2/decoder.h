//
//		CPSC 501 -- Assignment 2
//
//	Genetic Algorithm: This is intended to be a pluggable decoder from 
//		bit strings into useful forms. Incomplete.
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------
#ifndef DECODER_H
#define DECODER_H


#include "genome.h"

using namespace std;


//================================================================================
//
class	GenomeDecoder
{
//--------------------------------------------------------------------------------
//
	public:
		GenomeDecoder( int number_parameters, int size_parameters )
		: _code_ptr( NULL ), _irange_start( 0 ), _irange_end( 0 ), 
		  _frange_start( 0.0 ), _frange_end( 0.0 )
		{
			_number_parm	= number_parameters;
			_size_parm	= size_parameters;		
		}
		
		
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void	setRange( int start, int end )
		{	
			_irange_start 	= start;
			_irange_end	= end;
		}
		
		void	setRange( float start, float end )
		{	
			_frange_start 	= start;
			_frange_end	= end;
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		int	getIntParameter( Genome* g, int i )
		{
			int result	= 0;
			int end		= ( i+1 ) * _size_parm;
			
			_code_ptr = g->getCode();
			
			for( int n = i*_size_parm; n < end; n++ )
			{
				result << 1;
				result |= (*_code_ptr)[n];
				
				cout << result;
			} cout << endl;
			
			return result;
		}		


		float	getFloatParameter( Genome* g, int ith )
		{
			float result	= 0.0;
			
			return result;
		}		
		
	
	
//--------------------------------------------------------------------------------
//	
	private:
		vector<bool>*	_code_ptr;
		
		int		_number_parm;
		int		_size_parm;
		
		int		_irange_start;
		int		_irange_end;
		
		float		_frange_start;
		float		_frange_end;
};


#endif
