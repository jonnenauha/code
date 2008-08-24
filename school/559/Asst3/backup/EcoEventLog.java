//================================================================================
//
//
//
//================================================================================


package	Asst3;


import util.*;
import Asst3.*;

import java.util.LinkedList;
import java.util.ListIterator;

import java.io.PrintStream;


//--------------------------------------------------------------------------------
//
public class	EcoEventLog
{
	public		EcoEventLog()
	{
		log = new LinkedList();
	}
	
	public	void	log( EcoEvent e )
	{
		log.addLast( e );
	}
	
	public	void	dump( PrintStream p )
	{
		p.println( 
				"\nLog Dump on " + p + 
				"\n----------\n" 
			);
		
		ListIterator i = log.listIterator();
		while( i.hasNext() )
		{
			EcoEvent e = (EcoEvent) i.next();
			
			p.println( e );
		}
		
		p.println( "----------\n" );
	}
	
	private	LinkedList	log	= null;
}
