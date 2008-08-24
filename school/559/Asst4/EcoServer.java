//================================================================================
//
//
//
//================================================================================


package Asst4;


import util.*;
import Asst4.*;

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
				try
				{
					EcoServer es = new EcoServer( args );
					es.serve();
				}
				catch( Exception e )
				{
					new ExceptionHandler( e );
				}
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
	public			EcoServer( String[] args ) throws EcoInvalidServerArgumentsException
	{
		build_server_locations( args );
		build_server_channels();
		build_cc_monitors();
		
		local_time		= new Date();
		local_timestamp		= new EcoTimestamp( locations.length );
		event_log		= new EcoEventLog();	
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
					int event_id = 69; //Integer.parseInt( stdin.readln() );
					System.out.println( "69" );
				
					System.out.print( "Enter description: " );
					String description = stdin.readln();
					
					local_timestamp.increment( locations[HERE] );
					
					EcoEvent event = new EcoEvent( 
										locations[HERE], 
										event_id, 
										local_time.toString(), 
										description, 
										new EcoTimestamp( local_timestamp )
									);
					
					broadcast_event( event );
					
					break;
					
				case 2:
					collect_events();
					event_log.dump( System.out );
					cc_queue.dump( System.out );
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
		cc_monitor_active =  false;
		
		for( int i=0; i < channels.length; i++ )
		{
			channels[i].close();
		}
		
		System.out.println( "Main server " + locations[HERE] + " shutting down." );
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	void			build_server_locations( String[] args )	throws EcoInvalidServerArgumentsException
	{
		if( ( args.length & 1 ) != 0 )
		{
			throw new EcoInvalidServerArgumentsException();
		}
		else
		{
			locations = new EcoLocation[ args.length / 2 ];
		
			int i = 0;
			int j = 0;
		
			while( i < locations.length )
			{
				locations[i] = new EcoLocation( args[j], args[j+1] );
				System.out.println( locations[i] );
				i = i + 1;
				j = j + 2;
			}
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
	
	
	private	void			build_cc_monitors()
	{
		cc_queue		= new EcoPriorityQueue();
		
		cc_monitor_active	= true;
		cc_monitor		= new EcoCCMonitor( cc_queue );
	}
	
	
	private	void			broadcast_event( EcoEvent event )
	{		
		for( int i=0; i < channels.length; i++ )
		{
			channels[i].send( event );
		}
		
		event_log.log( event );
	}
	
	
	private	void			collect_events()
	{
		EcoEvent event = null;
		
		for( int i=0; i < channels.length; i++ )
		{
			do
			{
				event = channels[i].receive();
				cc_queue.priority_insert( event );
			}
			while( event != null );
		}
	}
	
	
	
	private	Date			local_time		= null;
	private	EcoTimestamp		local_timestamp		= null;
		
	private	EcoLocation[]		locations		= null;
	private	EcoRemoteChannel[]	channels		= null;
	private	EcoEventLog		event_log		= null;
	
	private	EcoPriorityQueue	cc_queue		= null;
	private	EcoCCMonitor		cc_monitor		= null;
	private	boolean			cc_monitor_active	= false;
	
	private	static final int	HERE			= 0;
	
	
	
	//--------------------------------------------------------------------------------
	//
	class 		EcoCCMonitor	extends Thread
	{
		public			EcoCCMonitor( Queue causal_queue )
		{
			this.causal_queue = causal_queue;
			
			yield();
			start();
		}
		
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//
		public	void		run()
		{
			while( cc_monitor_active )
			{
				collect_events();
				EcoEvent event	= ( EcoEvent ) causal_queue.peek();
			
				if( event != null )
				{
					EcoLocation location	= event.getLocation();
					EcoTimestamp timestamp	= event.getTimestamp();
			
					if( local_timestamp.causallyConsistent( location, timestamp ) )
					{
						local_timestamp.set( location, timestamp );
						event_log.log( event );
						causal_queue.pop();
					}
					else
					{}
				}
				else
				{}
				
				try
				{
					sleep( Math.round( Math.random() * 1000 ) );
				}
				catch( Exception e )
				{
					new ExceptionHandler( e );
				}
			}
			
			System.out.println( "CCMonitor deactivated." );
		}
		
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//
		private	Queue		causal_queue	= null;
	}		
}
