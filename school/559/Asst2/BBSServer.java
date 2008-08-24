//================================================================================
//
//
//
//================================================================================


package Asst2;

import Asst2.*;
import util.*;
import Asst2.identity.*;
import Asst2.rmiinterface.*;

import java.io.*;

import java.util.ArrayList;
import java.util.Hashtable;

import java.rmi.Naming;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.RMISecurityManager;



//================================================================================
public class BBSServer 		extends UnicastRemoteObject
				implements BBSServerRemote
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public static void	main( String args[] )
	{
		try
		{
			if( args.length != 1 )
			{
				System.out.println( "Usage: java Asst2/BBSServer <local_host_name>" );
			}
			else
			{
				BBSServer server = new BBSServer( args[0] );
				server.serve();
			}
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public			BBSServer( String hostname )	throws RemoteException
	{
		this.hostname	= hostname;
		
		stdin 		= new ReaderAdapter( System.in );
		stdout		= System.out;
		
		root_group	= new BBSGroupIdentity( "root" );
		message_list	= new ArrayList( 10 );
		user_list 	= new Hashtable( 10 );
		id_list		= new Hashtable( 10 );
		
		bind_to_registry( this, "BBSServer" );
		
		// Populate the Server with Data
		add_new_user( "root", "hi" );	
		add_new_user( "ryanm", "pass" );
		add_new_group( "group1" );
		add_new_group( "group2" );
		add_to_group( "root", "group1" );
		add_to_group( "ryanm", "group2" );
	}
	
	
	public	void	serve()
	{
		while( server_status > 0 )
		{
			stdout.println( "$" );
		}
	}
	
	
	public	boolean		login( String username, String password )
	{
		boolean valid = false;
		String pass = (String) user_list.get( username );
		
		if( pass != null )
		{
			if( pass.equals( password ) )
			{
				valid =  true;
			}
			else
			{
				stdout.println( "Wrong password." );
			}
		}
		else
		{
			stdout.println( "User not found." );
		}
		
		return valid;
	}
	
	
	public void		logout()
	{
	}
	
	
	public	int		post( String message )	
	{
		message_list.add( message );
		return message_list.indexOf( message );
	}
	
		
	public	void		edit( int i, String message )
	{
		String origional = (String) message_list.get( i );
		
		message_list.remove( i );
		message_list.add( i, origional + message );
	}
	
	
	public	void		delete( int i )	
	{
		message_list.remove( i );
	}
	
	
	public	Object[]	view()	
	{
		return message_list.toArray();
	}
	
	
	public void		send( String message, String name )
	{
		BBSIdentity id = get_id( name );
		
		if( id != null )
		{
			id.sendMail( message );
		}
		else
		{
			stdout.println( "Could not find " + name );
		}
	}
	
	
	public Object[]		receive( String name )
	{
		Object[]	mail	= null;
		BBSIdentity 	id 	= get_id( name );
		
		if( id != null )
		{
			mail = id.getMail();
		}
		else
		{
			stdout.println( "Could not find " + name + "." );
		}
		
		return mail;
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	public void		add_new_user( String username, String password )
	{
		user_list.put( username, password );
		id_list.put( username, new BBSUserIdentity( username ) );
		
		stdout.println( username + " added." );
		new CollectionPrinter( "----\n\nID List", id_list.values() );
		new CollectionPrinter( "Group List", root_group.getUsers() );
	}
	
	
	public void		add_new_group( String groupname )
	{
		BBSGroupIdentity groupid = new BBSGroupIdentity( groupname );
		
		root_group.add( groupid );
		id_list.put( groupname, groupid );
		
		stdout.println( groupname + " added." );
		new CollectionPrinter( "----\n\nID List", id_list.values() );
		new CollectionPrinter( "Group List", root_group.getUsers() );
	}
	
	
	public void		add_to_group( String name, String group )
	{
		BBSIdentity		id 	= (BBSIdentity) id_list.get( name );
		BBSGroupIdentity	groupid = (BBSGroupIdentity) id_list.get( group );
		
		if( id != null )
		{
			if( groupid != null )
			{
				groupid.add( id );	
			}
			else
			{
				root_group.add( id );
			}
			
			stdout.println( name + " added to " + group + "." );
		}
		else
		{
			stdout.println( "Could not find " + name + "." );
		}
		
		new CollectionPrinter( "----\n\nID List", id_list.values() );
		new CollectionPrinter( "Group List", root_group.getUsers() );
		new CollectionPrinter( "Group users", groupid.getUsers() );	
	}
	
	
	public void		remove_user( String name, String group )
	{
		BBSIdentity		id 	= (BBSIdentity) id_list.get( name );
		BBSGroupIdentity	groupid = (BBSGroupIdentity) id_list.get( group );
		
		groupid.remove( id );
		id_list.remove( id );
		user_list.remove( name );
		
		new CollectionPrinter( "----\n\nID List", id_list.values() );
		new CollectionPrinter( "Group List", root_group.getUsers() );
		new CollectionPrinter( "Group users", groupid.getUsers() );	
	}
	
	
	public BBSIdentity	get_id( String name )
	{
		BBSIdentity id = null;
		
		try
		{
			id = (BBSIdentity) id_list.get( name );
		}
		catch( Exception e ) 
		{
			new ExceptionHandler( e );
		}
		
		return id;	
	}
	
	
	private void	bind_to_registry( Remote obj, String name )
	{
	
		try 
		{
			stdout.print( "rmi://" + hostname + "/" + name );
			Naming.rebind( "rmi://" + hostname + "/" + name, obj );
			stdout.println( " bound in RMI Registry" );	
		} 
		catch( Exception e ) 
		{
			new ExceptionHandler( e );
		}
	}
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	BBSGroupIdentity	root_group	= null;
	private ArrayList		message_list	= null;
	private	Hashtable		user_list	= null;
	private Hashtable		id_list		= null;
	
	private String			hostname	= null;
	private int			server_status	= 0;
	
	private ReaderAdapter		stdin		= null;
	private PrintStream		stdout		= null;
}
