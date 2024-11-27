all: test

test: memory.o test.o
	gcc -Wno-deprecated-declarations -o test memory.o test.o

memory.o: memory.c
	gcc -c memory.c

test.o: test.c
	gcc -c test.c

clean:
	rm -f *.o test