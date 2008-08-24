//================================================================================
//
//
//
//================================================================================


package Asst4;


import Asst4.*;
import util.*;

import java.io.Serializable;


//--------------------------------------------------------------------------------
//
public class 			EcoTimestamp	implements Serializable
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			EcoTimestamp( int size )
	{
		vector	= new int[ size ];
	}
	
	
	public			EcoTimestamp( EcoTimestamp ts )
	{
		vector	= new int[ ts.vector.length ];
		update( ts );
	}
	
	
	public			EcoTimestamp( int v[] )
	{
		vector	= new int[ v.length ];
		update( v );
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public	boolean		equals( EcoTimestamp t )
	{
		boolean equals	= true;
		
		for( int i=0; i < vector.length; i++ )
		{
			if( vector[i] != t.vector[i] )
			{
				equals = false;
				break;
			}
		}
		
		return equals;
	}
	
	
	public	boolean		less( EcoTimestamp t )
	{
		boolean less	= true;
		
		for( int i=0; i < vector.length; i++ )
		{
			if( vector[i] > t.vector[i] )
			{
				less = false;
				break;
			}
			else
			{}
		}
		
		return less;
	}
	
	
	public	void		update( EcoTimestamp ts )
	{
		for( int i=0; i < vector.length; i++ )
		{
			if( vector[i] < ts.vector[i] )
			{
				vector[i] = ts.vector[i];
			}
			else
			{}
		}
	}
	
	
	public	void		update( int[] v )
	{
		for( int i=0; i < vector.length; i++ )
		{
			if( vector[i] < v[i] )
			{
				vector[i] = v[i];
			}
			else
			{}
		}
	}

	
	public	void		increment( EcoLocation l )
	{
		vector[ l.getLocationID() ] = vector[ l.getLocationID() ] + 1;
	}
	
	
	public	void		set( EcoLocation l, int value )
	{
		vector[ l.getLocationID() ] = value;
	}
	
	
	public	void		set( EcoLocation l, EcoTimestamp t )
	{
		vector[ l.getLocationID() ] = t.vector[ l.getLocationID() ];
	}
	
	
	
	public	int		get( EcoLocation l )
	{
		return vector[ l.getLocationID() ];
	}
	
	
	public	String		toString()
	{
		String s = new String( "< " );
		
		for( int i=0; i < vector.length; i++ )
		{
			s = s.concat( vector[i] + ", " );
		}
		
		return s.concat( ">" );
	}
	
	
	public	boolean		causallyConsistent( EcoLocation l, EcoTimestamp ts )
	{
		boolean consistent = false;
		int k = l.getLocationID();

		if( ts.vector[k] == vector[k] + 1 )
		{
			consistent = true;
			
			for( int i=0; i < vector.length; i++ )
			{
				if(( i != k )&&( ts.vector[i] > vector[i] ))
				{
					consistent = false;
					
					System.out.println( 
								this + " is not consistent with " + ts + 
								" since it knows about events to come."
							);
					break;
				}
			}
		}
		else
		{
			System.out.println( 
						this + " is not consistent with " + ts + 
						" since it doesnt immediately follow." 
					);
		}
		
		return consistent;
	}
	
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	int[]		vector		= null;
}
