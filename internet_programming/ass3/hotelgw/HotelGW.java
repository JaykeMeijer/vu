import java.net.MalformedURLException;
import java.rmi.Naming;
import java.lang.Integer;
import java.io.*;
import java.net.*;

public class HotelGW {
    public static void main (String [] args)
    {
        Hotel h;
        int nread = 0;
        String inputStr;
        String[] clientArgs;
        
        /* catch sockets creation/connecting
         * with error*/
        try {
            ServerSocket welcomeSocket = new ServerSocket(1234);
            welcomeSocket.setReuseAddress(true);

            /* connect to the rmi server using localhost
             * as default or first given argument */
            if(args.length > 0)
                h = (Hotel)Naming.lookup("rmi://" + args[0] + "/hotelimp");
            else
                h = (Hotel)Naming.lookup("rmi://localhost/hotelimp");

            while(true) {
                try {
                    /* accept incomming clients */
                    Socket socket = welcomeSocket.accept();
                    DataInputStream in = new DataInputStream(socket.getInputStream());
                    DataOutputStream out = new DataOutputStream(socket.getOutputStream());

                    /* get input/command from user */
                    byte[] input = new byte[256];
                    nread = in.read(input);

                    /* split the arguments on spaces */
                    inputStr = new String(input);
                    clientArgs = (inputStr.replaceAll("\0", "")).split(" ");

                    /* check arguments, run corresponding command
                     * and send answer back to the client */
                    if(clientArgs[0].equals("list")) {
                        inputStr = h.list();
                        out.writeBytes(inputStr);
                    } else if(clientArgs[0].equals("book")) {
                        String name = "";
                        for(int i = 2; i < clientArgs.length; i++)
                            name += clientArgs[i] + " ";
                        name.substring(0, name.length() - 1);
                        out.writeBytes(h.book(Integer.parseInt(clientArgs[1]), name).toString());
                    } else if(clientArgs[0].equals("guests")) {
                        System.out.println("HERE WE GOOOO");
                        out.writeBytes(h.guests());
                    }

                    /* close all file descriptors */
                    in.close();
                    out.close();
                    socket.close();
                } catch(Exception e) {
                    System.out.println("Exception: " + e);
                }
            }
        } catch(Exception e) {
            System.out.println("Exception: " + e);
        }
    }
}
