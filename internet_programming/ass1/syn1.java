/*  Assignment 1 - Unix Multiprocessing
 *  syn1.java 
 *
 * author:   Rik van der Kooij
 * VUnet-ID: rkj800
 * date:     21-09-2012
 */

public class syn1 {
    public static Object lock = new Object();

    public static void main(String[] args) {
        /* create thread objects */
        syn1.PrintThread hello = new syn1.PrintThread("Hello World\n");
        syn1.PrintThread bonjour = new syn1.PrintThread("Bonjour monde\n");

        hello.start();
        bonjour.start();
    }

    /* display function revised for java */
    public static void display(String str)
    { 
        for(int i = 0; i < str.length(); i++) {
            System.out.write(str.charAt(i));
            try {
                Thread.sleep(0, 100000);
            } catch (Exception e) {
                System.out.println(e);
            }
        }
    }

    /* thread class for displaying the strings */
    private static class PrintThread extends Thread {
        String message;

        PrintThread(String str) {
             message = str;   
        }

        public void run() {
            for(int i = 0; i < 10; i++) {
                /* acquire lock object before displaying */
                synchronized(syn1.lock) {
                    syn1.display(message);
                }
            }
        }
    }
}
