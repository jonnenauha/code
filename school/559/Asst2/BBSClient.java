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
import java.util.Stack;

import java.rmi.Naming;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.RMISecurityManager;



//================================================================================
public class BBSClient
{
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	public static void	main( String args[] )
	{
		if( args.length != 1 )
		{
			System.out.println( "Usage: java Asst2/BBSClient <remote_host_name>" );
		}
		else
		{
			String 	remote_host = args[0];
		
			BBSClient client = new BBSClient( remote_host );
			client.run();
		}	
	}
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	public			BBSClient( String hostname )
	{
		stdin 		= new ReaderAdapter( System.in );
		stdout		= System.out;
		
		post_history	= new Stack();
		
		this.hostname	= hostname;
		
		setup_security_manager();
		setup_server();
	}
	
	
	//
	public void		run()
	{
		print_title_ui();
		
		int login_tries = 0;

		do
		{
			if( login_tries < 3 )
			{
				login_tries++;
			}
			else
			{
				return ;
			}
		}
		while( !login_command() );
		
		
		int choice = 0;
		while ( choice < 8 )
		{
			print_menu_ui();
			choice = Integer.parseInt( stdin.readln() );
					
			switch( choice )
			{
				case 1:	// 
					post_command();
					break;
						
				case 2:	//
					edit_command();
					break;
					
				case 3:	// 
					view_command();
					break;
						
				case 4:	//
					delete_command();
					break;
						
				case 5:	//
					send_command();
					break;
				
				case 6: //
					receive_command();
					break;
				
				case 7: //
					admin_command();
					break;
					
				case 8: //
					logout_command();
					break;			
						
				default: // Error
					stdout.println( "Unknown command." );
					choice = 0;
					break;
			}		
		}			
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private void	print_title_ui()
	{
		System.out.println
		( 
			"\n\t eBBS v0.2 \t\t -- \"Serving the BS community since 2002.\"\n" +
			"---------------------------------------------------------------------------"
		);	
	}
	
	
	//	
	private void	print_menu_ui()
	{
		System.out.print
		(
			"\t( 1 )  Post to eBBS.\n" +
			"\t( 2 )  Edit a post.\n" +
			"\t( 3 )  View all posts.\n" +
			"\t( 4 )  Delete a post.\n" +
			"\t( 5 )  Send to identity.\n" +
			"\t( 6 )  Receive message.\n" +
			"\t( 7 )  Administrative commands.\n" +
			"\t( 8 )  Log out of eBBs.\n" +
			" $> "
		);
	}
	
	
	//
	private boolean	login_command()
	{
		boolean valid	= false;
		String username	= new String();
		String password	= new String();
		
		try
		{
			System.out.print( "Login to " + hostname + " as username: " );
			username = stdin.readln();
			
			System.out.print( "Please give " + username + "'s password: " );
			password = stdin.readln();
		
			valid = server.login( username, password );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
		
		return valid;	
	}
	
	
	//
	private void	logout_command()
	{
		try
		{
			stdout.println( "Good Bye!" );
			server.logout();
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//
	private void	post_command()
	{
		try
		{	
			stdout.print( "Enter text line to post: " );
			String 	text	= stdin.readln();
			
			int post_num	= server.post( text );
			stdout.println( "Post " + post_num + " added." );
			
			post_history.push( new Integer( post_num ) );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}		
	}
	
	
	//
	private void	edit_command()
	{
		try
		{	
			stdout.print( "Enter post to edit: " );
			int post_num 	= Integer.parseInt( stdin.readln() );
		
			stdout.print( "Enter text to append: " );
			String message	= stdin.readln();
		
			server.edit( post_num, message );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//
	private void	view_command()
	{
		try
		{		
			Object[] messages = server.view();
			
			for( int i=0; i < messages.length; i++ )
			{
				stdout.println( "Message " + i + ": " + (String) messages[i] );
			}
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}		
	}
	
	
	private void	delete_command()
	{
		try
		{	
			stdout.print( "Enter post to delete: " );
			int post_num 	= Integer.parseInt( stdin.readln() );
		
			server.delete( post_num );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}		
	}
	
	
	//
	private	void	send_command()
	{
		String message 	= null;
		String name	= null;
		
		try
		{
			stdout.print( "Enter message: " );
			message = stdin.readln();
			stdout.print( "Enter user to send to: " );
			name = stdin.readln();
			
			server.send( message, name );
		}
		catch ( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//
	private void	receive_command()
	{
		String 		name = null;
		Object[]	mail = null;
		
		try
		{
			stdout.print( "Enter mailbox to read: " );
			name = stdin.readln();
			mail = server.receive( name );
			
			for( int i=0; i < mail.length; i++ )
			{
				stdout.println( (String) mail[i] );
			}
		}
		catch ( Exception e )
		{
			new ExceptionHandler( e );
		}		
	}
	
	
	//
	private void	admin_command()
	{
		int choice	= 0;
		String password	= new String();
		
		try
		{
			System.out.print( "Login to " + hostname + " as root: " );
			password = stdin.readln();
		
			if( server.login( "root", password ) )
			{
			
				while( choice < 4 )
				{
					stdout.print
					(
						"\n\tAdministrator commands.\n" +
						"----------------------------------------\n" +
						"\t( 1 )  Add a new Group.\n" +
						"\t( 2 )  Add a new User.\n" +
						"\t( 3 )  Remove a user.\n" +
						"\t( 4 )  Return to main menu.\n" +
						" $> "
					);
			
					choice = Integer.parseInt( stdin.readln() );
					switch( choice )
					{
						case 1: //
							addgroup_command();
							break;
				
						case 2: //
							adduser_command();
							break;
					
						case 3: //
							removeuser_command();
							break;
							
						case 4: //
							break;
					
						default: //
							stdout.println( "Unknown command" );
							choice = 0;
							break;
					}
				}
			}
			else
			{
				stdout.println( "Administrative access required." );
			}
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}			
	}
	
	
	//
	private void	addgroup_command()
	{
		String group = null;
		
		try
		{
			stdout.print( "Enter group to add: " );
			group = stdin.readln();
			
			server.add_new_group( group );
		}
		catch ( Exception e )
		{
			new ExceptionHandler( e );
		}		
	}
	
	
	//
	private void	adduser_command()
	{
		String user 	= null;
		String password	= null;
		String group 	= null;
		
		try
		{
			stdout.print( "Enter user to add: " );
			user	= stdin.readln();
			
			stdout.print( "Enter new password: " );
			password = stdin.readln();
			
			stdout.print( "Enter group to add [default:root]: " );
			group	= stdin.readln();
					
			server.add_new_user( user, password );
			server.add_to_group( user, group );
		}
		catch ( Exception e )
		{
			new ExceptionHandler( e );
		}			
	}
	
	
	private void	removeuser_command()
	{
		String user	= null;
		String group	= null;
		
		try
		{
			stdout.print( "Enter user to remove: " );
			user = stdin.readln();
			
			stdout.print( "Enter the group the user belongs to: " );
			group = stdin.readln();
			
			server.remove_user( user, group );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//
	private void	setup_server()
	{
		String service 	= new String( "BBSServer" );
		String rmi_name = new String( "//" + hostname + "/" + service );
		
		System.out.println( "Looking up RMI object " + rmi_name );
		
		try
		{
			server = (BBSServerRemote) Naming.lookup( rmi_name );
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	//
	private void	setup_security_manager()
	{
	
		if( System.getSecurityManager() == null ) 
		{
		    System.setSecurityManager( new RMISecurityManager() );
		}
		else
		{}
	}	
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	private	BBSServerRemote	server		= null;
	
	private String		hostname	= null;
	private Stack		post_history	= null;
	
	private ReaderAdapter	stdin		= null;
	private PrintStream	stdout		= null;
}

