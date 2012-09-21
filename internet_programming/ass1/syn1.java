public class syn1 {
    public static Object lock = new Object();

    public static void main(String[] args) {
        syn1.PrintThread hello = new syn1.PrintThread("Hello World\n");
        syn1.PrintThread bonjour = new syn1.PrintThread("Bonjour monde\n");

        hello.start();
        bonjour.start();
    }

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

    private static class PrintThread extends Thread {
        String message;

        PrintThread(String str) {
             message = str;   
        }

        public void run() {
            for(int i = 0; i < 10; i++) {
                synchronized(syn1.lock) {
                    syn1.display(message);
                }
            }
        }
    }
}
