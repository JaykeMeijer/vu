class HotelImp extends java.rmi.server.UnicastRemoteObject implements Hotel
{
    Room[] r;

    public HotelImp() throws java.rmi.RemoteException {
        super();

        r = new Room[3];
        r[0] = new Room(10, 150);
        r[1] = new Room(20, 120);
        r[2] = new Room(20, 100);
    }

    public String list() throws java.rmi.RemoteException {
        String s = "";

        for(int i = 0; i < r.length; i++)
            s += r[i].guests.length - r[i].noOfGuests +  "\t";
        s.substring(0, s.length() - 1);

        return s;
    }
    public Boolean book(int type, String name) throws java.rmi.RemoteException {
        if(type > r.length)
            return false;
        return r[type - 1].add(name);
    }

    public String guests() throws java.rmi.RemoteException {
        String s = "";

        for(int i = 0 ; i < r.length; i++)
            s += r[i].getGuests();
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

        public Boolean add(String name) {
            if(noOfGuests == guests.length)
                return false;
            guests[noOfGuests] = name;
            noOfGuests++;
            return true;
        }

        public String getGuests() {
            String s = "";

            for(int i = 0; i < noOfGuests; i++)
                s += guests[i] + '\n';
            return s;
        }
    }
}
