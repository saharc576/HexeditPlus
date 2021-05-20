all: exec

exec: task4.c
	gcc -g -m32 -c -fno-pie -o task4.o task4.c
	gcc -g -m32 task4.o -fno-pie -o dc
	rm task4.o

.PHONY: clean
clean:
	rm -rf ./*.o dc