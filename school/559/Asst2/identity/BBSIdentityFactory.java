//
//	Assignment 1 Multi-threaded Server
//
//
//							Ryan McDougall -- 2002
//================================================================================


package Asst2.identity;


public static class BBSIdentityFactory
{
	public		BBSIdentityFactory()
	{
		identifier = 1;
	}
	
	
	public static int	createUniqueID()
	{
		return identifier++;
	}
	
	private static int	identifier;
}
