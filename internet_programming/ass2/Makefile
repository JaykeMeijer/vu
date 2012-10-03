CC=gcc
EXE=client serv1 serv2 serv3 talk
HEADER=writen_error

all: writen $(EXE)

%: %.c 
	$(CC) -c $^
	$(CC) $@.o $(HEADER).o -o $@
	
writen:
	$(CC) -c $(HEADER).c

clean:
	rm $(EXE) *.o
