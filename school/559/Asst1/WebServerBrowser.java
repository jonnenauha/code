//
//	Assignment 1 Socket Browser
//
//
//							Ryan McDougall -- 2002
//--------------------------------------------------------------------------------


import java.io.*;
import java.net.*;
import java.util.StringTokenizer;


//================================================================================
// This is the instanciating class. Its sole job is to instanciate and manipulate
// The SocketReader class.
//
public class WebServerBrowser
{
	public 		WebServerBrowser()
	{
	}
	
	public static void main( String[] args )
	{
		SocketReader readerInstance = new SocketReader( args[0] );
		readerInstance.start();
	}
}


//================================================================================
// This class is derived from, but not the previous SocketReader. It implements
// the complementary functionality of the WebServer.
//
class SocketReader
{
	public static final int WEB_PORT = 8888;


	// Establish connection and build Reader/Writer
	public 		SocketReader( String host )
	{
		this.host = host;
	}
	
		
	// Offer a minimal user interface and call the respective private helper functions
	public void 	start()
	{
		int command		= 0;
		BufferedReader stdin 	= new BufferedReader( 
						new InputStreamReader( System.in ) 
						);
		try
		{			
			do
			{
				//------ HACK!!
				try
				{
					server = InetAddress.getByName( host );
					System.out.println( "Looking for: " + server );
			
					connection = new Socket( server, WEB_PORT );
					System.out.println( "Connected: " + connection );
					
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
				}
				catch( Exception e )
				{
					e.printStackTrace();
				}	
				//------ END HACK.
			
				print_user_interface();
				command = Integer.parseInt( stdin.readLine() );
				output.println( command );
				
				switch( command )
				{
					case 1:	// Send file
						System.out.print( "Enter local file to send: " );
						send_file( stdin.readLine() );
						
						break;
						
					case 2:	// List directory
						System.out.println( "Enter remote directory to list: " );
						output.println( stdin.readLine() );
						list_directory();
						break;
					
					case 3:	// Check file
						System.out.println( "Enter remote file to check: " );
						output.println( stdin.readLine() );
						check_file();
						break;
						
					case 4:	// Create file
						System.out.println( "Enter remote file to create: " );
						output.println( stdin.readLine() );
						create_file();
						break;
						
					case 5:	// Delete file
						System.out.println( "Enter remote file to delete: " );
						output.println( stdin.readLine() );
						delete_file();
						break;
					
					case 6:	// Append file
						System.out.println( "Enter remote file to append: " );
						output.println( stdin.readLine() );
						System.out.println( "Enter text to append: " );
						output.println( stdin.readLine() );
						append_file();
						break;
					
					case 7:	// Exit client
						break;
						
					default: // Error
						System.out.println( "Command Error." );
						output.println( stdin.readLine() );
						break;
				}				
			}
			while( command < 7 );											
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
	// Private Members
	
	
	// Close any open resources
	private void	close()
	{
		try
		{
			connection.close();
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
	}
	
	
	// Open a Reader to the given file and send the string to the server
	private void	send_file( String path )
	{
		String local_output =  new String();
		File local_file = new File( path );
		
		try
		{
			BufferedReader local_file_reader = new BufferedReader( 
								new FileReader( local_file )
								);
			
			// Send the filename first					
			output.println( local_file.getName() );								
		
			// Then the file text
			while( ( local_output = local_file_reader.readLine() ) != null )
				output.println( local_output );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}		
	}
	
	
	// All below functions simply dump server replies to stdout
	private void	list_directory()
	{
		print_reply();	
	}
	
	
	private void	check_file()
	{
		print_reply();
	}
	
	
	private void	create_file()
	{
		print_reply();
	}
	
	private void	delete_file()
	{
		print_reply();
	}
	
	private void	append_file()
	{
		print_reply();
	}
	
	
	// Dump server stream to stdout
	private void	print_reply()
	{
		String listing =  new String();
		try
		{
			while( ( listing = input.readLine() ) != null )
				System.out.println( listing );
			
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}		
	}
	
	
	// Print a minimal user interface
	private void	print_user_interface()
	{
		System.out.print( 
					"\nServer Browser 0.1\n----------------------------------------\n" +
					"\t( 1 )  Send local file.\n" +
					"\t( 2 )  List remote directory file.\n" +
					"\t( 3 )  Check remote file.\n" +
					"\t( 4 )  Create remote file.\n" +
					"\t( 5 )  Delete remote file.\n" +
					"\t( 6 )  Append remote file.\n" +
					"\t( 7 )  Exit.\n"
				);
	}
	
	
	String 			host;
	private	InetAddress 	server;
	private Socket		connection;
	private BufferedReader 	input;
	private	PrintWriter	output;	
}
