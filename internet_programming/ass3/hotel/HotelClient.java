import java.net.MalformedURLException;
import java.rmi.Naming;
import java.lang.Integer;

public class HotelClient {
    public static void main (String [] args)
    {
        if(args.length < 2)
            commands();
        else {
            try {
                Hotel h = (Hotel)Naming.lookup("rmi://" + args[0] + "/hotelimp");

                if(args[1].equals("list"))
                    System.out.println(h.list());
                else if(args[1].equals("book"))
                    System.out.println(h.book(Integer.parseInt(args[2]), args[3]));
                else if(args[1].equals("guests"))
                    System.out.print(h.guests());
                else
                    commands();
            } catch(Exception e) {
                System.out.println("Exception: " + e);
            }
        }
    }

    public static void commands() {
        System.out.println("PRINT COMMANDLINE OPTINOS");
        
    }

}
