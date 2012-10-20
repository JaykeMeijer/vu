CC		:= gcc
CFLAGS  := -Wall

all: paperclient paperserver

clean:
	rm -f paperclient.o add_clnt.o add_xdr.o add_svc.o paperserver.o add_clnt.c add.h add_xdr.c add_svc.c paperclient paperserver

paperclient: paperclient.o add_clnt.o add_xdr.o 
	$(CC) -o paperclient paperclient.o add_clnt.o add_xdr.o -lnsl  

paperserver: paperserver.o add_svc.o add_xdr.o
	$(CC) -o paperserver paperserver.o add_svc.o add_xdr.o -lnsl 

add_clnt.c add.h add_xdr.c add_svc.c: add.x
	rpcgen add.x

paperclient.o: paperclient.c add.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

add_clnt.o: add_clnt.c add.h
	$(CC) $(CFLAGS) -c -o add_clnt.o add_clnt.c

add_xdr.o: add_xdr.c add.h
	$(CC) $(CFLAGS) -c -o add_xdr.o add_xdr.c

add_svc.o: add_svc.c add.h
	$(CC) $(CFLAGS) -c -o add_svc.o add_svc.c

paperserver.o: paperserver.c add.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c
	

run:
#!	./paperserver &
	./paperclient localhost add 'rik 1' 'test1' ../../papers/binary.pdf
	./paperclient localhost add 'rik 2' 'test2' ../../papers/binary.pdf
