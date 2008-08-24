//================================================================================
//
//
//
//================================================================================


package Asst3b;


import util.*;
import Asst3b.*;

import java.rmi.Remote;
import java.rmi.RemoteException;


//--------------------------------------------------------------------------------
//
interface	EcoToken	extends Remote
{
	public	void	passToken()	throws RemoteException;
	public	void	acceptToken()	throws RemoteException;
}
