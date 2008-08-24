//================================================================================
//
//
//
//================================================================================


package util;

import java.util.Collection;


//--------------------------------------------------------------------------------
//
public class CollectionPrinter
{
	public	CollectionPrinter( String title, Collection c )
	{
		System.out.println( title );
		
		Object[] obj = c.toArray();
		
		for( int i=0; i < obj.length; i++ )
		{
			System.out.println( obj[i].toString() );
		}
	}
	
	public	CollectionPrinter( String title, Object[] obj )
	{
		System.out.println( title );
		
		for( int i=0; i < obj.length; i++ )
		{
			System.out.println( obj[i].toString() );
		}
	}	
}
