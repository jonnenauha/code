//================================================================================
//
//
//
//================================================================================


package Asst2.rmiinterface;

import Asst2.*;
import Asst2.util.*;
import Asst2.identity.*;
import Asst2.rmiinterface.*;


import java.rmi.Remote;
import java.rmi.RemoteException;


//================================================================================
public interface BBSServerRemote	extends Remote
{
	public	int		post( String message )				throws RemoteException;
	public	void		edit( int i, String message )			throws RemoteException;
	public	void		delete( int i )					throws RemoteException;
	public 	Object[]	view()						throws RemoteException;
	
	public	void		send( String message,String id )		throws RemoteException;
	public 	Object[]	receive( String name )				throws RemoteException;	
	
	public void		remove_user( String name, String group )	throws RemoteException;	
	public void		add_new_user( String name, String pass )	throws RemoteException;
	public void		add_to_group( String name, String group )	throws RemoteException;
	public void		add_new_group( String groupname )		throws RemoteException;
	public BBSIdentity	get_id( String name )				throws RemoteException;
	
	public	boolean		login( String username, String password )	throws RemoteException;
	public	void		logout()					throws RemoteException;
}
