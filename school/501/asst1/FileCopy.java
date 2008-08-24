//
//		CPSC 501 -- Assignment 1
//
//	Comparision of interpreted language with C.
//
//			Copyright: Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------


import java.lang.String;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;


//--------------------------------------------------------------------------------
public class FileCopy
{
	public static void	main( String args[] )
	{
		if( args.length == 2 )
		{
			FileCopy app = new FileCopy( args[0], args[1] );
		}
		else
		{
			System.out.println( "Usage: FileCopy <SRC> <DST>" );
		}
	}
	
	//--------------------------------------------------------------------------------
	public		FileCopy( String source, String destination )
	{
		try
		{
			int block_size		= 4096;
			char buffer[]		= new char[ block_size ];			
		
			FileReader src_reader	= new FileReader( new File( source ) );
			FileWriter dst_writer	= new FileWriter( new File( destination ) );
			
			long start_time		= System.currentTimeMillis();
			long total_time		= 0;
		
			while( src_reader.read( buffer, 0, block_size ) != -1 )
			{	
				dst_writer.write( buffer, 0, buffer.length );
			}
			
			total_time = System.currentTimeMillis() - start_time;
			
			System.out.println( "Copy took " + total_time + "ms." );
			
			src_reader.close();
			dst_writer.close();
				
		}
		catch( java.lang.Exception e )
		{
			System.out.println( e.getMessage() );
			e.printStackTrace();
		}
	}
}
