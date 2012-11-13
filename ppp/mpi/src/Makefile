CC := 	 	 gcc
MPICC := 	 mpicc
SOURCEDIR := src
BUILDDIR :=  build

all: tab.o dist_seq dist_par

tab.o: tab.c tab.h
	$(CC) -c tab.c

dist_seq: dist_seq.c
	$(CC) dist_seq.c tab.o -o ../build/dist_seq

dist_par: dist_par.c
	$(MPICC) dist_par.c tab.o -o ../build/dist_par

clean:
	rm *.o ../build/dist_seq ../build/dist_par
