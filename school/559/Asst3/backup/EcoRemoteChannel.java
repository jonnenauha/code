//================================================================================
//
//
//
//================================================================================


package Asst3;


import util.*;
import Asst3.*;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.RMISecurityManager;
import java.rmi.server.UnicastRemoteObject;

import java.util.LinkedList;


//--------------------------------------------------------------------------------
//
public class	EcoRemoteChannel	implements EcoBidirectionalChannel
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			EcoChannel( EcoLocation source, EcoLocation destination )
	{
		try
		{
			this.source 		= source;
			this.destination 	= destination;
		
			producer 		= new Queue();
			consumer 		= new Queue();
		
			dispatcher_active 	= true;
			dispatcher 		= new EcoChannelDispatcher( producer );
			dispatcher.start();
			
			receiver_active		= true;
			receiver		= new EcoChannelReceiver( consumer );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public	void		send( EcoEvent e )
	{
		producer.push( e );
	}
	
	public	EcoEvent	receive()
	{
		EcoEvent e = ( EcoEvent ) consumer.peek();
		consumer.pop();

		return e;
	}
	
	public	void		close()
	{
		dispatcher_active	= false;
		receiver_active		= false;
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	EcoLocation	source		= null;
	private	EcoLocation	destination	= null;
	
	private	Queue		producer	= null;
	private Queue		consumer	= null;
	
	private	EcoChannelDispatcher	dispatcher		= null;
	private	boolean			dispatcher_active	= false;
	
	private	EcoChannelReceiver	receiver		= null;
	private	boolean			receiver_active		= false;
	
	
	//--------------------------------------------------------------------------------
	//
	class 		EcoChannelDispatcher	extends Thread
	{
		public			EcoChannelDispatcher( Queue dispatch_queue )
		{
			this.dispatch_queue = dispatch_queue;
			yield();
		}
	
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//
		public	void		run()
		{
			while( dispatcher_active )
			{
				for( int i=0; dispatch_queue.size() > 0; i++ )
				{
					EcoEvent e = (EcoEvent) dispatch_queue.peek();
					dispatch_queue.pop();
					
					System.out.println( "Dispatching event: " + e + "." );
					new EcoChannelTransport( e, destination );
				}
				
				try
				{
					sleep( 1000 );
				}
				catch( Exception e )
				{
					System.out.println( "Dispatcher awakened." );
					new ExceptionHandler( e );
				}
			}
			
			System.out.println( "Dispatcher deactivated." );
		}

		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//
		private	Queue		dispatch_queue		= null;
	}
	
	
	//--------------------------------------------------------------------------------
	//
	class 		EcoChannelReceiver	extends 	UnicastRemoteObject
						implements 	EcoRemoteReceiver
	{
		public			EcoChannelReceiver( Queue receive_queue )	throws RemoteException
		{
			super();
			
			this.receive_queue = receive_queue;
			setup_rmi();
		}
		
		public	void		accept( EcoEvent e )
		{
			if( receiver_active )
			{
				System.out.println( "Received: " + e + " on " + source );
				receive_queue.push( e );
			}
			else
			{}
		}
		
		private	void		setup_rmi()
		{
			String hostname = new String( "localhost" );

 			if( System.getSecurityManager() == null ) 
			{
			    System.setSecurityManager( new RMISecurityManager() );
			}			
			
			try
			{
				Naming.rebind( "rmi://" + hostname + "/" + destination, this );			
			}
			catch( Exception e )
			{
				new ExceptionHandler( e );
			}
		}
		
		private	Queue		receive_queue	= null;
	}
}



//--------------------------------------------------------------------------------
//
class		EcoChannelTransport	extends Thread
{
	public			EcoChannelTransport( EcoEvent message, EcoLocation location )
	{
		this.message	= message;
		this.location	= location;

	 	setup_rmi();
		run();
	}
	
	public	void		run()
	{
		try
		{
			remote.accept( message );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
		
		System.out.println( "Sent: " + message + " to " + location );
	}
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	
	private	void		setup_rmi()
	{
		String hostname	= new String( "localhost" );
		
		try
		{
			remote = (EcoRemoteReceiver) Naming.lookup("rmi://" + hostname + "/" + location ); 
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	private	EcoLocation		location	= null;
	private	EcoEvent		message		= null;
	private	EcoRemoteReceiver	remote		= null;
}
