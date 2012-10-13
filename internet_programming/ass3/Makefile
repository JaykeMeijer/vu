CC=gcc
EXE=paperclient
HEADER=writen_error

all: writen $(EXE)

%: %.c 
	$(CC) -c $^
	$(CC) $@.o $(HEADER).o -o $@
	
writen: $(HEADER).c $(HEADER).h
	$(CC) -c $(HEADER).c

clean:
	rm $(EXE) *.o
