public class syn2 {
    public static volatile int ab = 1;
    public static volatile int cd = 0;

    public static void main(String[] args) {
        syn2.PrintThread hello = new syn2.PrintThread("ab");
        syn2.PrintThread bonjour = new syn2.PrintThread("cd\n");

        hello.start();
        bonjour.start();
    }
    
    public static void lockDisplay(String message) {
        Boolean a = message == "ab";

        try {
            for(int i = 0; i < 10; i++) {
                if(a)
                    while(ab == 0);
                else
                    while(cd == 0);
                syn2.display(message);
                if(a) {
                    ab = 0;
                    cd = 1;
                } else {
                    cd = 0;
                    ab = 1;
                }
            }
        }
        catch(Exception e) {
            System.out.println(e);
        }
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
            syn2.lockDisplay(this.message);
        }
    }
}
