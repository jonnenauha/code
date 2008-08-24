//
//	Assignment 1 Multi-threaded Server
//
//
//							Ryan McDougall -- 2002
//================================================================================


package util;


import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.OutputStream;


//--------------------------------------------------------------------------------
//
public class PrinterAdapter
{
	public		PrinterAdapter( File file )
	{
		try
		{
			printer = new PrintWriter( 
					new BufferedWriter(
						new FileWriter( file )
						),
						true
					);
		}
		catch( Exception e )
		{
			new ExceptionHandler( e );
		}		
	}
	
	
	public		PrinterAdapter( OutputStream ostream )
	{
		printer = new PrintWriter( 
				new BufferedWriter(
					new OutputStreamWriter( ostream	)
					),
					true
				);
	}
	
	
	public void	println( String line )
	{
		printer.println( line );
	}
	
	private PrintWriter	printer;	
}
