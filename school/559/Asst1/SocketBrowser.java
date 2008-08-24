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
public class SocketBrowser
{
	public 		SocketBrowser()
	{
	}
	
	public static void main( String[] args )
	{
		SocketReader readerInstance = new SocketReader();
		readerInstance.start();
	}
}


//================================================================================
// This class has one public method, start(), which provides a minimal user 
// interface and basic network functionality. Namely implementing aribitrary 
// HTTP/1.0 commands.
//
class SocketReader
{
	public static final int WEB_PORT = 80;

	//----------------------------------------
	//
	public 		SocketReader()
	{
	}
	
		
	public void 	start()
	{
		String httpVersion	= new String( "HTTP/1.0\n" );
		String protocol		= new String( "http://" );
		String url 		= new String();
		String host		= new String();
		String command		= new String();
		String reply		= new String();
		BufferedReader stdin 	= new BufferedReader( 
						new InputStreamReader( System.in ) 
						);
		try
		{
			System.out.print( "Please enter the URL you wish to browse: " );
			url = stdin.readLine();	
			
			// This extracts the hostname from the URL
			StringTokenizer st = new StringTokenizer( url );
			host = st.nextToken( "/" );
			
			System.out.print( "Please enter the HTTP command you wish to issue: " );
			command = stdin.readLine();
			
			// Resolve IP address by hostname
			server = InetAddress.getByName( host );
			System.out.println( "Looking: " + server );
			
			// Make socket connection to the socket
			connection = new Socket( server, WEB_PORT );
			System.out.println( "Connected: " + connection );
			
			// Build buffered Readers and Writers
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
			
			// Issue the command
			output.println( command + " " + protocol + url + " " + httpVersion );
				
			// Echo response
			System.out.println( "Sent command: " + command + " " + protocol + url + " " + httpVersion );
			System.out.println( "Server replied: " );
			
			while(( reply = input.readLine() ) != null )
			    System.out.println( reply );
			   											
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}
		finally
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
	}
	
	private	InetAddress 	server;
	private Socket		connection;
	private BufferedReader 	input;
	private	PrintWriter	output;	
}
