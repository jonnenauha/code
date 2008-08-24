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
public class	EcoRemoteChannel	implements EcoChannel
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			EcoRemoteChannel( EcoLocation local, EcoLocation remote )
	{
		try
		{
			this.local 		= local;
			this.remote 		= remote;
		
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
			System.out.println( "Unable to create a remote Receiver:" );
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
	private	EcoLocation	local		= null;
	private	EcoLocation	remote		= null;
	
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
				try
				{
					while( dispatch_queue.size() > 0 )
					{
						EcoEvent event = (EcoEvent) dispatch_queue.peek();
						
						new EcoChannelTransport( event, remote );
					
						dispatch_queue.pop();
					}
				}
				catch( EcoTransportUnavailableException e )
				{
					// Event not sent due to unavailable Transport -- will retry later.
				}
				
				try
				{
					sleep( Math.round( Math.random() * 1000 ) );
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
		private	int		tries			= 0;
	}
	
	
	//--------------------------------------------------------------------------------
	//
	class 		EcoChannelReceiver	extends 	UnicastRemoteObject
						implements 	EcoRemoteReceiver
	{
		public			EcoChannelReceiver( Queue receive_queue )	throws RemoteException
		{
			this.receive_queue = receive_queue;
			setup_rmi();
		}
		
		public	void		accept( EcoEvent event )
		{
			if( receiver_active )
			{
				System.out.println( "Received: " + event + " on " + local );
				receive_queue.push( event );
			}
			else
			{}
		}
		
		private	void		setup_rmi()
		{
 			if( System.getSecurityManager() == null ) 
			{
			    System.setSecurityManager( new RMISecurityManager() );
			}			
			
			try
			{
				Naming.rebind( "rmi://" + local.getHostname() + "/" + local, this );			
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
	public			EcoChannelTransport( EcoEvent message, EcoLocation destination )
						throws EcoTransportUnavailableException
	{
		this.message		= message;
		this.destination	= destination;

	 	setup_rmi();
		start();
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
		
		System.out.println( "Sent: " + message + " to " + destination );
	}
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	
	private	void		setup_rmi()	throws EcoTransportUnavailableException
	{
		String rmipath	= new String( "rmi://" + destination.getHostname() + "/" + destination );
		
		try
		{
			remote = (EcoRemoteReceiver) Naming.lookup( rmipath ); 
		}
		catch( Exception e )
		{
			// Destination not bound in rmipath
			throw new EcoTransportUnavailableException();
		}
	}
	
	private	EcoLocation		destination	= null;
	private	EcoEvent		message		= null;
	private	EcoRemoteReceiver	remote		= null;
}



//--------------------------------------------------------------------------------
//
class EcoTransportUnavailableException extends Exception
{
	// Empty
}
