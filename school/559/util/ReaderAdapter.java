//
//	Assignment 1 Multi-threaded Server
//
//
//							Ryan McDougall -- 2002
//================================================================================


package util;


import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.io.File;
import java.io.FileReader;

import util.ExceptionHandler;


//--------------------------------------------------------------------------------
//
public class ReaderAdapter
{
	public		ReaderAdapter( File file )
	{
		try
		{
			reader = new BufferedReader( 
					new FileReader( file )
					); 
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}
	}
	
	
	public		ReaderAdapter( InputStream istream )
	{
		reader 	= new BufferedReader( 
					new InputStreamReader( istream ) 
					);
	}
	
	
	public String	readln()
	{
		String line = new String();
		
		try
		{
			line = reader.readLine();
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
			line = null;
		}
		
		return line;
	}
	
	private	BufferedReader	reader;	
}
