//================================================================================
//
//
//
//							Ryan McDougall -- 2002
//================================================================================


package util;


//--------------------------------------------------------------------------------
//
public class ExceptionHandler
{
	public 	ExceptionHandler( Exception e )
	{
		this.e = e;
		this.print();
	}
	
	void	print()
	{
		System.out.println( "Exception caught: \n" + e.getMessage() );
		this.e.printStackTrace();
	}
	
	private	Exception e;
}
