/*  Assignment 1 - Unix Multiprocessing
 *  syn2.java 
 *
 * author:   Rik van der Kooij
 * VUnet-ID: rkj800
 * date:     21-09-2012
 */

public class syn2 {
    /* mutex and flag object for mutual exclusion */
    public static Object mutex = new Object();
    public static Boolean flag = new Boolean(true);

    public static void main(String[] args) {
        /* create thread objects */
        syn2.PrintThread hello = new syn2.PrintThread("ab");
        syn2.PrintThread bonjour = new syn2.PrintThread("cd\n");

        hello.start();
        bonjour.start();
    }
    
    /* locking display function for thread printing "ab" */
    public static void lockDisplayAB(String message) {
        try {
            for(int i = 0; i < 10; i++) {
                /* acquire lock */
                synchronized(mutex) {
                    syn2.display(message);  /* display message */
                    flag = false;           /* set flag */
                    mutex.notify();         /* notify cd thread */
                    while(!flag)
                        mutex.wait();       /* wait for signal or flag */
                }
            }
        }
        catch(Exception e) {
            System.out.println(e);
        }
    }
    
    /* locking display function for thread printing "cd\n" */
    public static void lockDisplayCD(String message) {
        try {
            for(int i = 0; i < 10; i++) {
                /* acquire lock */
                synchronized(mutex) {
                    while(flag)             /* wait for signal or flag */
                        mutex.wait();
                    syn2.display(message);  /* display message */
                    flag = true;            /* set flag */
                    mutex.notify();         /* signal ab thread */
                }
            }
        }
        catch(Exception e) {
            System.out.println(e);
        }
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
            /* use the correct lockDisplayXX method */
            if(this.message == "ab")
                syn2.lockDisplayAB(this.message);
            else
                syn2.lockDisplayCD(this.message);
        }
    }
}
