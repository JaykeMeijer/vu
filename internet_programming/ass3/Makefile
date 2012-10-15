CC=gcc
EXE=paperclient
HEADER=headers/writen_error

all: writen $(EXE)

%: %.c 
	$(CC) -c $^
	$(CC) $@.o $(HEADER).o -o $@
	
writen: $(HEADER).c $(HEADER).h
	$(CC) -c $(HEADER).c -o $(HEADER).o

clean:
	rm $(EXE) *.o
