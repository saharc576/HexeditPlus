all: task1

task1: task1.o 
	gcc -g -m32 -Wall -o task1 task1.o

task1.o: task1.c task1.h
	gcc -g -m32 -Wall -c -o task1.o task1.c

.PHONY: clean

clean:
	rm -rf ./*.o task1