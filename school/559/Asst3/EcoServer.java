//================================================================================
//
//
//
//================================================================================


package Asst3;


import util.*;
import Asst3.*;

import java.util.Date;
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
		build_server_locations( args );
		build_server_channels();
		
		local_time	= new Date();
		event_log	= new EcoEventLog();	
	}
	
	
	public	void		serve()
	{
		ReaderAdapter stdin	= new ReaderAdapter( System.in );
		int command = 0;
		
		while( command < 3 )
		{
			System.out.println( " 1. Enter event.\n 2. Dump event log to screen. \n 3. Quit." );
			command = Integer.parseInt( stdin.readln() );
			
			switch( command )
			{
				case 1:
					System.out.print( "Enter event id: ");
					int event_id = Integer.parseInt( stdin.readln() );
				
					System.out.print( "Enter description: " );
					String description = stdin.readln();
				
					EcoEvent event = new EcoEvent( locations[HERE], event_id, local_time.toString(), description );
					
					event_log.log( event );
					broadcast_event( event );
					
					break;
					
				case 2:
					collect_events();
					event_log.dump( System.out );
					break;
					
				case 3:
					// Quit
					break;
					
				default:
					command = 0;
					break;
			}
		}
		
		destroy();
	}
	
	
	public	void		destroy()
	{
		for( int i=0; i < channels.length; i++ )
		{
			channels[i].close();
		}
		
		System.out.println( "Main server " + locations[HERE] + " shutting down." );
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	void			build_server_locations( String[] args )
	{
		ReaderAdapter stdin = new ReaderAdapter( System.in );	
		locations	= new EcoLocation[ args.length ];
		

		
		for( int i=0; i < locations.length; i++ )
		{
			System.out.print( "Enter server name for " + args[i] + " : " );
			
			locations[i] = new EcoLocation( args[i] );		
			locations[i].setHostname( stdin.readln() );
		}
	}
	
	private	void			build_server_channels()
	{
		channels	= new EcoRemoteChannel[ locations.length-1 ];
		
		for( int i=0; i < channels.length; i++ )
		{
			channels[i] = new EcoRemoteChannel( locations[HERE], locations[i+1] );
		}
	}
	
	private	void			broadcast_event( EcoEvent event )
	{
		for( int i=0; i < channels.length; i++ )
		{
			channels[i].send( event );
		}
	}
	
	private	void			collect_events()
	{
		EcoEvent event = null;
		
		for( int i=0; i < channels.length; i++ )
		{
			do
			{
				event = channels[i].receive();
				event_log.log( event );
			}
			while( event != null );
		}
	}
	
	private	Date			local_time	= null;
	private	EcoLocation[]		locations	= null;
	private	EcoRemoteChannel[]	channels	= null;
	private	EcoEventLog		event_log	= null;
	
	private	static final int	HERE		= 0;
}
