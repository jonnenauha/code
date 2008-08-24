//
//	Assignment 1 Multi-threaded Server
//
//
//							Ryan McDougall -- 2002
//================================================================================


package Asst2.identity;


import java.util.ArrayList;

import Asst2.*;
import Asst2.util.*;
import Asst2.identity.*;
import Asst2.rmiinterface.*;


//================================================================================
//
public class BBSGroupIdentity extends BBSIdentity
{
	//
	public 			BBSGroupIdentity( String name )
	{		
		super( name );
		
		group = new ArrayList( 10 );
	}
	
	
	//
	public BBSIdentity	lookup( BBSIdentity id )
	{
		BBSIdentity lookup_id = null;
		int position = group.indexOf( id );
		
		if( position >= 0 )
		{
			lookup_id = (BBSIdentity) group.get( position );
		}
		else
		{}
		
		return lookup_id;
	}
	
	
	//
	public void		add( BBSIdentity id )
	{
		if( group.indexOf( id ) < 0 )
		{
			group.add( id );
		}
		else
		{}
	}
	
	
	//
	public void 		remove( BBSIdentity id )
	{
		int position = group.indexOf( id );
		
		if( position >= 0 )
		{
			group.remove( position );
		}
		else
		{}
	}
	
	
	//
	public Object[]		getUsers()
	{
		return group.toArray();
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// Private

	private ArrayList	group			= null;
}
