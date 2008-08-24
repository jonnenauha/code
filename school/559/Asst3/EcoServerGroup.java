//================================================================================
//
//
//
//================================================================================


package Asst3;


import Asst3.*;
import util.*;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.ListIterator;


//--------------------------------------------------------------------------------
//
public class	EcoServerGroup	implements EcoUnidirectionalChannel
{
	public			EcoServerGroup( EcoLocation[] to_locations )
	{
		from_location = to_locations[0];
		channels = new ArrayList( to_locations.length );
		
		for( int i=0; i < to_locations.length; i++ )
		{
			channels.add( new EcoRemoteChannel( from_location, to_locations[i] ) );
		}
	}
	
	public	void		send( EcoEvent message )
	{
		EcoUnidirectionalChannel c	= null;
		ListIterator i 			= channels.listIterator();
		
		while( i.hasNext() )
		{
			c = (EcoUnidirectionalChannel) i.next();
			c.send( message );
		}
	}
	
	public	void		close()
	{
		EcoChannel c	= null;
		ListIterator i 	= channels.listIterator();
		
		while( i.hasNext() )
		{
			c = (EcoChannel) i.next();
			c.close();
		}		
	}
	
	private	EcoLocation	from_location	= null;
	private	ArrayList	channels	= null;
}
