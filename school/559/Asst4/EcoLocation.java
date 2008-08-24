//================================================================================
//
//
//
//================================================================================


package Asst4;


import java.io.Serializable;


//--------------------------------------------------------------------------------
//
final public class 	EcoLocation	implements Serializable
{
	public			EcoLocation( String location, String hostname )
	{
		this.location = location;
		this.hostname = hostname;
		 
		if( location.equals( "AF" ) )
			location_id = EcoLocations.AF;
			
		else if( location.equals( "AM" ) )
		 	location_id = EcoLocations.AM;
			
		else if( location.equals( "AS" ) )
		 	location_id = EcoLocations.AS;
			
		else if( location.equals( "EU" ) )
		 	location_id = EcoLocations.EU;
			
		else
		 	location_id = EcoLocations.LO;			
	}
	
	
	public	boolean		equals( EcoLocation el )
	{
		boolean equals = false;
		
		if( this.location_id == el.location_id )
		{
			equals = true;
		}
		
		return equals;
	}
	
	
	public	String		toString()
	{
		return new String( location + " at " + hostname );
	}
	
	
	public	String		getHostname()
	{
		return hostname;
	}
	

	public	String		getLocation()
	{
		return location;
	}

	
	public	int		getLocationID()
	{
		return location_id;
	}
	
	
	
	private	String	hostname	= null;
	private String	location	= null;
	private	int	location_id	= 0;
}


//--------------------------------------------------------------------------------
//
final class EcoLocations
{
	public static int LO = 0;
	public static int AF = 0;
	public static int AM = 1;
	public static int AS = 2;
	public static int EU = 3;
}
