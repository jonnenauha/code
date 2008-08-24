//================================================================================
//
//
//
//================================================================================


package Asst4;


import Asst4.*;
import util.*;

import java.rmi.Naming;


//--------------------------------------------------------------------------------
//
public class		EcoChannelTransport	extends Thread
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
		String rmipath	= new String( "rmi://" + destination.getHostname() + "/" + destination.getLocation() );
		
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

