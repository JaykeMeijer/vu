CC		:= gcc
CFLAGS  := -Wall

all: paperclient paperserver

clean:
	rm -f paperclient.o paper_clnt.o paper_xdr.o paper_svc.o paperserver.o paper_clnt.c paper.h paper_xdr.c paper_svc.c paperclient paperserver

paperclient: paperclient.o paper_clnt.o paper_xdr.o 
	$(CC) -o paperclient paperclient.o paper_clnt.o paper_xdr.o -lnsl  

paperserver: paperserver.o paper_svc.o paper_xdr.o
	$(CC) -o paperserver paperserver.o paper_svc.o paper_xdr.o -lnsl 

paper_clnt.c paper.h paper_xdr.c paper_svc.c: paper.x
	rpcgen paper.x

paperclient.o: paperclient.c paper.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

paper_clnt.o: paper_clnt.c paper.h
	$(CC) $(CFLAGS) -c -o paper_clnt.o paper_clnt.c

paper_xdr.o: paper_xdr.c paper.h
	$(CC) $(CFLAGS) -c -o paper_xdr.o paper_xdr.c

paper_svc.o: paper_svc.c paper.h
	$(CC) $(CFLAGS) -c -o paper_svc.o paper_svc.c

paperserver.o: paperserver.c paper.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c
	

run:
	./paperserver &
	./paperclient localhost add 'rik 1' 'test1' ../../p.pdf
	./paperclient localhost add 'rik 2' 'test2' ../../p.pdf
