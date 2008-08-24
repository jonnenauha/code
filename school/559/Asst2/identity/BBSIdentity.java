//
//	Assignment 1 Multi-threaded Server
//
//
//							Ryan McDougall -- 2002
//================================================================================


package Asst2.identity;

import java.util.ArrayList;


//================================================================================
public class BBSIdentity
{
	public			BBSIdentity( String name )
	{
		this.name = name;
		this.mail =  new ArrayList( 10 );
		
		unique_identifier = BBSIdentityFactory.createUniqueID();
	}
	
	public int		getIdentity()
	{
		return unique_identifier;
	}
	
	public String		getName()
	{
		return name;
	}
	
	public void		sendMail( String mail )
	{
		this.mail.add( mail );
	}
	
	public Object[]		getMail()
	{
		return mail.toArray();
	}
	
	public boolean		equals( BBSIdentity identity )
	{
		boolean equals = false;
		if( this.unique_identifier == identity.unique_identifier )
		{
			equals = false;
		}
		else
		{}
		
		return equals;
	}
	
	public String		toString()
	{
		return name;
	}
	
	
	private	ArrayList	mail			= null;
	
	private String		name			= null;
	private int 		unique_identifier	= 0;
}
