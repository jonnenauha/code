//================================================================================
//
//
//
//================================================================================


package Asst4;


import Asst4.*;
import util.*;

import java.util.LinkedList;
import java.util.ListIterator;
import java.io.PrintStream;


//--------------------------------------------------------------------------------
//
public class EcoPriorityQueue	extends Queue
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			EcoPriorityQueue()
	{
		super();
	}
	
	
	public	void		priority_insert( EcoEvent event )
	{
		if( event != null )
		{
			boolean inserted	= false;
			EcoTimestamp p		= null;
			EcoTimestamp t		= event.getTimestamp();
			ListIterator i		= list.listIterator( HEAD );
		
			while( i.hasNext() && !inserted )
			{
				p = ( (EcoEvent) i.next() ).getTimestamp();
			
				if( t.less( p ) )
				{
					i.previous();
					i.add( event );
					inserted = true;
				}
				else
				{}
			}
		
			if( !inserted )
			{
				push( event );
			}
			else
			{}
		}
		else
		{}
	}
	

	public	void	dump( PrintStream p )
	{
		p.println( 
				"\nEcoPriorityQueue Dump on " + p + 
				"\n----------\n" 
			);
		
		EcoEvent e	= null;
		ListIterator i = list.listIterator();
		
		while( i.hasNext() )
		{
			e = (EcoEvent) i.next();
			p.println( e );
		}
		
		p.println( "----------\n" );		
	}	
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private final int	HEAD	= 0;
}
