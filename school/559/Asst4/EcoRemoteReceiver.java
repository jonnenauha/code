//================================================================================
//
//
//
//================================================================================


package Asst4;


import java.rmi.Remote;
import java.rmi.RemoteException;


//--------------------------------------------------------------------------------
//
public interface	EcoRemoteReceiver	extends Remote
{
	public void	accept( EcoEvent e )	throws RemoteException;
}
