class HotelImp extends java.rmi.server.UnicastRemoteObject implements Hotel
{
    Room[] r;

    public HotelImp() throws java.rmi.RemoteException {
        super();

        /* create three simple rooms */
        r = new Room[3];
        r[0] = new Room(10, 150);
        r[1] = new Room(20, 120);
        r[2] = new Room(20, 100);
    }

    /* Lists the number of rooms available 
     * in each price range */
    public String list() throws java.rmi.RemoteException {
        String s = "";

        for(int i = 0; i < r.length; i++)
            s += r[i].guests.length - r[i].noOfGuests +  "\t";
        return s.substring(0, s.length() - 1);
    }

    /* Books a room of the specified type
     * (if available) on the given name */
    public Boolean book(int type, String name) throws java.rmi.RemoteException {
        if(type > r.length)
            return false;
        return r[type - 1].add(name);
    }

    /* Lists the names of all registered guests */
    public String guests() throws java.rmi.RemoteException {
        String s = "";

        /* loop trough all room and get a
         * string of the guests in that room */
        for(int i = 0 ; i < r.length; i++)
            s += r[i].getGuests();

        /* remove last tab */
        if(s.length() > 1)
            return s.substring(0, s.length() - 1); 
        else
            return s;
    }


    class Room {
        String guests[];
        int price,
            noOfGuests = 0; 
        
        public Room(int capacity, int price) {
            guests = new String[capacity];
            this.price = price;
        }

        /* add a new guest to the room */
        public Boolean add(String name) {
            if(noOfGuests == guests.length)
                return false;
            guests[noOfGuests] = name;
            noOfGuests++;
            return true;
        }

        /* returns a string containing all
         * guests seperated with a newline */
        public String getGuests() {
            String s = "";

            for(int i = 0; i < noOfGuests; i++)
                s += guests[i] + '\n';
            return s;
        }
    }
}
