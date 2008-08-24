//
//	Assignment 1 Multi-threaded Server
//
//
//							Ryan McDougall -- 2002
//--------------------------------------------------------------------------------


import java.io.*;
import java.io.File.*;
import java.net.*;


//================================================================================
// This is the instanciating class. Its sole job is to instanciate and manipulate
// The MultiServer class.
//
public class WebServer
{
	//----------------------------------------
	//
	public		WebServer(){}
	public static 	void main( String[] args )
	{
		MultiServer serverInstance = new MultiServer( args[0], TEST_PORT );
		serverInstance.serve();
	}
	
	//----------------------------------------
	//	
	private static int 	TEST_PORT = 8888;
}



//================================================================================
// This class creates a new thread for each incoming client via the inner class
// ServerThread. ServerThread implements the actual services, MultiServer only
// Provides a mechanism for creating and controlling the threads
//
class MultiServer
{

	//________________________________________________________________________________
	// Inner class is supposed to monitor the behavior of ServerThreads and
	// accept command line input on behalf of the server. Doesnt work.
	class MonitorThread extends Thread
	{
		public		MonitorThread()
		{
			try
			{
				// Access standard input
				stdin = new BufferedReader( 
						new InputStreamReader( System.in ) 
					);
				
				// Begin monitoring command line
				monitorCommandline();
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}			
		}
		
		public void	monitorCommandline()
		{
			String command = new String();
			
			try
			{
				// Watch for the internal command "exit"
				while( server_status > 0 )
				{
					System.out.print( "$" );
					command = stdin.readLine();
					
					if( command.equals( "exit" ) )
						server_status = 0;
				}
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}
		}
		
		private	BufferedReader	stdin;
	}
	
	
	//________________________________________________________________________________
	// This class implements the main functionality for the webserver via the public 
	// method serve(). Serve accepts and integer command and runs the respective
	// private helper function.
	//
	class ServerThread extends Thread
	{
		public		ServerThread( ThreadGroup group, Socket incoming )
		{
			connection = incoming;
			
			try
			{
				// Build Readers and Writers from the Socket
				input = new BufferedReader( 
							new InputStreamReader( 
								connection.getInputStream()
							)
						);
		
				output = new PrintWriter( 
							new BufferedWriter(
								new OutputStreamWriter(
									connection.getOutputStream()
								)
							),
							true
						);
						
				// Begin servicing clients
				serve();
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}			
		}
		
		
		public void	serve()
		{
			int command = 0;
			
			try
			{	
				command = Integer.parseInt( input.readLine() );
					
				switch( command )
				{
					case 1:	// Receive file
						receive_file();
						break;
						
					case 2:	// List directory
						list_directory();
						break;
					
					case 3:	// Check file
						check_file();
						break;
						
					case 4:	// Create file
						create_file();
						break;
						
					case 5:	// Delete file
						delete_file();
						break;
					
					case 6:	// Append file
						append_file();
						break;
					
					case 7:	// Exit
						break;
						
					default: // Error
						break;
				}
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}
			finally
			{					
				close();
			}
		}
		
		
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// Private helper functions the implement Server functionality.
		
		
		// Close any open resources
		private void	close()
		{
			try
			{
				connection.close();
				this.join();
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}
		}
		
		
		// Read file from client and print it locally
		private void	receive_file()
		{
			String remote_input =  new String();

			try
			{
				File local_file = new File( input.readLine() );
			
				PrintWriter local_file_writer = new PrintWriter(
									new BufferedWriter(
										new FileWriter( local_file )
									)
								);
									
				while( ( remote_input = input.readLine() ) != null )
					local_file_writer.println( remote_input );
					
				local_file_writer.close();				
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}				
		}
		
		
		// Build a file array from which to iterate and print properties
		private	void	list_directory()
		{
			try
			{
				File local_directory = new File( input.readLine() );
				File[] listing = local_directory.listFiles();
				
				for( int i=0; i < listing.length; i++ )
				{
					
					output.println( 
							listing[i].getName() + "\t\t" + 
							"d: " + listing[i].lastModified()  + "\t" +
							"s: " + listing[i].length() + "\t" +
							"r: " + listing[i].canRead() + " " +
							"w: " + listing[i].canWrite()
						);
				}
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}			
		}
		
		
		// Print to the client the properties of a given file
		private void	check_file()
		{
			try
			{
				File local_file = new File( input.readLine() );
				if( local_file.exists() )
					output.println(
							local_file.getName() + "\t\t" + 
							"d: " + local_file.lastModified()  + "\t" +
							"s: " + local_file.length() + "\t" +
							"r: " + local_file.canRead() + " " +
							"w: " + local_file.canWrite()
						);					
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}			
		}
		
		
		// Create new file from given filename
		private void	create_file()
		{
			try
			{
				File local_file = new File( input.readLine() );
				local_file.createNewFile();
				
				if( local_file.exists() )
					output.println( local_file.getName() + " has been created." );
				else
					System.out.println( "File Doesnt Exist!" );					
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}			
		}
		
		
		// Delete file from given filename
		private void	delete_file()
		{
			try
			{
				File local_file = new File( input.readLine() );
				local_file.delete();
				
				if( !local_file.exists() )
					output.println( local_file.getName() + " has been deleted." );
				else
					System.out.println( "File Wasnt Deleted!" );			
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}				
		}
		
		
		// Append given string to given file
		private	synchronized void append_file()
		{
			String remote_input =  new String();
			try
			{
				File local_file = new File( input.readLine() );
			
				PrintWriter local_file_writer = new PrintWriter(
									new BufferedWriter(
										new FileWriter( local_file )
									)
								);
									
				local_file_writer.println( input.readLine() );
					
				local_file_writer.close();
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}
		}
		
		
		
		private	int		thread_id;
		private	Socket		connection;
		private BufferedReader 	input;
		private PrintWriter 	output;		
	}
	
	//________________________________________________________________________________
	// This class serves only to construct Threads upon request.
	//
	public		MultiServer( String host, int port )
	{
		int back_log	= 0;
		working_port 	= port;
		server_status 	= 1;
		thread_count	= 0;
		next_thread_id	= 0;
	
		try
		{
			// Build a ServerSocket to listen from incoming connection requests
			InetAddress local_host = InetAddress.getByName( host );
			listener = new ServerSocket( working_port, back_log, local_host );
			System.out.println( "Listening for connections on " + listener );
			
			active_threads = new ThreadGroup( "active_threads" );
			//monitor = new MonitorThread();
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
	}
	
	
	// Server blocks until a new connection is requested, then spawns Thread
	public void 	serve()
	{
		while( server_status > 0 )
		{
			try
			{
				new ServerThread( active_threads, listener.accept() );
			}
			catch( Exception e )
			{
				e.printStackTrace();
			}
		}

		destroy();
	}

	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// Private helper functions
	//
	
	
	// Destroy the Server. Close any open resources.
	private void 	destroy()
	{
		try
		{
			listener.close();
			System.out.println( "No longer listening for connections on " + listener );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}		
				 
		if( active_threads != null );
		{
			System.out.println
			( 
				"Destroying active Thread Group " + active_threads +
				" with " + active_threads.activeCount() + " active threads." 
			);
			
			active_threads.destroy();
		}
	}
	
	
	private ServerSocket	listener;
	private MonitorThread	monitor;
	private ThreadGroup 	active_threads;
	private	int	server_status;
	private int	working_port;
	private int	thread_count;
	private int	next_thread_id;
	
}
