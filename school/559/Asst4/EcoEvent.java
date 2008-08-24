//================================================================================
//
//
//
//================================================================================


package Asst4;


import util.*;
import Asst4.*;

import java.io.Serializable;


//--------------------------------------------------------------------------------
//
public class 	EcoEvent	implements Serializable
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			EcoEvent( EcoLocation location, int event_id, String local_time, String description, EcoTimestamp timestamp )
	{
		this.location		= location;
		this.event_id		= event_id;
		this.local_time		= local_time;
		this.description	= description;
		this.timestamp		= timestamp;
	}
	
	
	public	EcoLocation	getLocation()
	{	return location;	}
	
	public	void		setLocation( EcoLocation l )
	{	location = l;		}
	
	
	public	int		getEventID()
	{	return event_id;	}
	
	public	void		setEventID( int e )
	{	event_id = e;		}
	
	
	public	String		getLocalTime()
	{	return local_time;	}
	
	public	void		setLocalTime( String lt )
	{	local_time = lt;	}
	
	
	public	String		getDescription()
	{	return description;	}
	
	public	void		setDescription( String d )
	{	description = d;	}
	
	
	public	EcoTimestamp	getTimestamp()
	{
		return timestamp;
	}
	
	public	void		setTimestamp( EcoTimestamp t )
	{
		timestamp = t;
	}
	
	
	public	String	toString()
	{
		return new String( 
					"< " + location + ", " + event_id + ", " + 
					local_time + ", " + description + " >" +
					" at " + timestamp
				);
		
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	EcoTimestamp	timestamp	= null;
	private	EcoLocation	location	= null;
	
	private	String		local_time	= null;
	private	String		description	= null;
	
	private	int		event_id	= 0;
}
