//================================================================================
//
//
//
//================================================================================


package Asst3;


import java.io.Serializable;


//--------------------------------------------------------------------------------
//
final public class 	EcoLocation	implements Serializable
{
	public			EcoLocation( String location )
	{
		this.location = location;
		 
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
		return location;
	}
	
	
	public	String		getHostname()
	{
		return hostname;
	}
	
	public	void		setHostname( String h )
	{
		hostname = h;
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
	public static int AF = 1;
	public static int AM = 2;
	public static int AS = 3;
	public static int EU = 4;
}
