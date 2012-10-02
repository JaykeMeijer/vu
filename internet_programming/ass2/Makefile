CC=gcc
EXE=client serv1 serv2 serv3 talk

all: $(EXE)

%: %.c
	$(CC) $^ -o $@ -g
	
clean:
	rm $(EXE)
