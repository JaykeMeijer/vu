CC=gcc

all: 
	$(CC) -Wall -c -o papers.o papers.c
	$(CC) -o papers papers.o ../paper/paper_xdr.o ../paper/paper_clnt.o -lnsl
	$(CC) -Wall -c -o paperview.o paperview.c
	$(CC) -o paperview paperview.o ../paper/paper_xdr.o ../paper/paper_clnt.o -lnsl

clean:
	rm papers
