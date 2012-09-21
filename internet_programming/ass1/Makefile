CC=gcc

all: mysh syn java

mysh:
	$(CC) mysh1.c -o mysh1
	$(CC) mysh2.c -o mysh2
	$(CC) mysh3.c -o mysh3

syn:
	$(CC) syn1.c -o syn1
	$(CC) syn2.c -o syn2
	$(CC) synthread1.c -o synthread1 -plthread
	$(CC) synthread2.c -o synthread2 -plthread

java:
	javac syn1.java
	javac syn2.java

clean:
	rm mysh1 mysh2 mysh3 syn1 syn2 synthread1 synthread2 *.class
