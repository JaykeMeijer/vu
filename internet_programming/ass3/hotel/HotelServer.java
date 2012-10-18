import java.rmi.Naming;

public class HotelServer {
    public HotelServer() {
        try {
            Hotel h = new HotelImp();
            Naming.rebind("rmi://localhost/hotelimp", h);
        } catch(Exception e) {
            System.out.println("Trouble: " + e);
        }
    }

    public static void main (String [] args)
    {
        new HotelServer();
    }
}
