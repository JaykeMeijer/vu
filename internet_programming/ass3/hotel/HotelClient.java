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
                /* try to connect to the rmi server */
                Hotel h = (Hotel)Naming.lookup("rmi://" + args[0] + "/hotelimp");

                /* check arguments, give corresponding command
                 * to rmi server and print the answer */
                if(args[1].equals("list"))
                    System.out.println(h.list());
                else if(args[1].equals("book") && args.length > 3) {
                    String name = "";
                    for(int i = 3; i < args.length; i++)
                        name += args[i] + " ";
                    name.substring(0, name.length() - 1);
                    h.book(Integer.parseInt(args[2]), name);
                } else if(args[1].equals("guests"))
                    System.out.println(h.guests());
                else
                    /* no correct command is found */
                    commands();
            } catch(NumberFormatException e) {
                /* given type is not an integer */
                commands();
            } catch(Exception e) {
                System.out.println("Exception: " + e);
            }
        }
    }

    /* displays the available command-line options available */
    public static void commands() {
        System.out.println("usage: ");
        System.out.println("hotelclient <hostname> list                      : lists the number of rooms available");
        System.out.println("hotelclient <hostname> book <type> 'guest name'  : book a room. type is a number");
        System.out.println("hotelclient <hostname> guests                    : lists the names of all registered guests");
    }

}
