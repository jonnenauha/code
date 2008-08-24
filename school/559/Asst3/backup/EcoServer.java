//================================================================================
//
//
//
//================================================================================


package Asst3;


import util.*;
import Asst3.*;

import java.util.LinkedList;


//--------------------------------------------------------------------------------
//
public class	EcoServer	extends Thread
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public static	void	main( String[] args )
	{
		if( args.length > 0 )
		{
			if( args[0].length() == 2 )
			{
				EcoServer es = new EcoServer( args );
				es.serve();
				es.destroy();
			}
			else
			{
				System.out.println( "Wrong server name." );
			}
		}
		else
		{
			System.out.println( "Need server name." );
		}
		
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			EcoServer( String[] args )
	{
		EcoLocation[] locations = new EcoLocation[ args.length-1 ];
		
		for( int i=0; i < locations.length; i++ )
		{
			locations[i] = new EcoLocation( args[i+1] );
		}
		
		local		= new EcoRemoteChannel( 
		replicas	= new EcoServerGroup( locations );
		event_log	= new EcoEventLog();		
	}
	
	
	public	void		serve()
	{
		for( int i=0; i < 5; i++ )
		{
			EcoEvent out_event = new EcoEvent( location, 69, 1200 + i, "Owwwgh... My brain got shot!" );
			EcoEvent in_event =  null;
			
			try
			{
				sleep( 3000 );
			}
			catch( Exception e )
			{
				System.out.println( "Main server woken." );
				new ExceptionHandler( e );
			}
			
			do
			{
				in_event = local.receive();
			}
			while( in_event == null );
			
			event_log.log( in_event );
			event_log.dump( System.out );
		}
	}
	
	
	public	void		destroy()
	{
		replicas.close();
		System.out.println( "Main server " + location + " shutting down." );
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	EcoEventLog		event_log	= null;
	private	EcoLocation		location	= null;
	private	EcoRemoteChannel	local_channel		= null;
	private	EcoServerGroup		replica_channels	= null;
}
