//
//	Assignment 1 URL Browser
//
//
//							Ryan McDougall -- 2002
//--------------------------------------------------------------------------------


import java.io.*;
import java.net.*;


//================================================================================
// This is the instanciating class. Its sole job is to instanciate and manipulate
// The URLReader class.
//
public class URLBrowser
{
	public URLBrowser()
	{
	}
	
	public static void main( String[] args )
	{
		URLReader readerInstance = new URLReader();
		readerInstance.start();
	}
}


//================================================================================
// This class has one public method, start(), which provides a minimal user 
// interface and basic network functionality. Namely implementing aribitrary 
// HTTP/1.0 commands.
//
class URLReader
{
	public URLReader()
	{
		try
		{
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}			
	}
	
	public void start()
	{
		String httpVersion	= new String( "HTTP/1.0\n" );
		String protocol		= new String( "http://" );
		String url 		= new String();
		String command		= new String();
		String reply		= new String();
		BufferedReader stdin 	= new BufferedReader( 
						new InputStreamReader( System.in ) 
						);
		try
		{
		
			System.out.print( "Please enter the URL you wish to browse: " );
			url = stdin.readLine();
			
			System.out.print( "Please enter the HTTP command you wish to issue: " );
			command = stdin.readLine();			
			
			// Make the Resource Location
			URL resource = new URL( protocol + url );				
					
			// Open an implementation of the abstract class HttpURLConnection
			// which provides output services via getOutputStream() and
			// set its properties to allow output
			location = (HttpURLConnection)resource.openConnection();
			location.setDoOutput(true);
			location.setDoInput(true);

			output = new PrintWriter(
					new BufferedWriter(
						new OutputStreamWriter(
							location.getOutputStream()
							)
						),
						true
					);
			
			// Make the actual connection to the resource
			location.connect();
			
			// Get a Reader for input
			input = new BufferedReader( 
					new  InputStreamReader( 
						location.getInputStream() 
						)
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
			// Nothing to close
		}	
	}
	
	private HttpURLConnection	location;
	private BufferedReader 		input;
	private	PrintWriter		output;
}
