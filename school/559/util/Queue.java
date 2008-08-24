//================================================================================
//
//
//
//================================================================================


package util;


import java.util.LinkedList;


//--------------------------------------------------------------------------------
//
public class 		Queue
{
	public		Queue()
	{
		list = new LinkedList();
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//	
	public	Object	peek()
	{
		Object obj = null;
		
		if( list.size() > 0 )
		{
			obj = list.getFirst();
		}
		
		return obj;
	}
	
	
	public	void	pop()
	{
		if( list.size() > 0 )
		{
			list.removeFirst();
		}
	}
	
	
	public	void	push( Object o )
	{
		if( o != null )
		{
			list.addLast( o );
		}
	}
	
	
	public	int	size()
	{
		return list.size();
	}
	
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//
	protected LinkedList	list	= null;
}
