/* Hotel interface with the 3 methods
 * specified in the assignment */
public interface Hotel extends java.rmi.Remote
{
    public String list()
        throws java.rmi.RemoteException;

    public Boolean book(int type, String name)
        throws java.rmi.RemoteException;
    
    public String guests()
        throws java.rmi.RemoteException;
}
