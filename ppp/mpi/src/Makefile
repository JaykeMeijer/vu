CC := 	 	 gcc
MPICC := 	 mpicc
SOURCEDIR := src
BUILDDIR :=  build

all: dist_seq dist_par

dist_seq: tab.o dist_seq.c
	$(CC) dist_seq.c tab.o -o ../build/dist_seq

dist_par: tab.o dist_par.c
	$(MPICC) dist_par.c tab.o -o ../build/dist_par

tab.o: tab.c tab.h
	$(CC) -c tab.c

#!tab1d.o: tab1d.c tab1d.h
#!	$(CC) -c tab.c

clean:
	rm *.o ../build/dist_seq ../build/dist_par
