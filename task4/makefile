all: task4

task4: task4.o 
	gcc -g -m32 -Wall -o task4 task4.o

task4.o: task4.c task4.h
	gcc -g -m32 -Wall -c -o task4.o task4.c

.PHONY: clean

clean:
	rm -rf ./*.o task4