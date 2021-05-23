all: exec

exec: ntsc.c
	gcc -g -m32 -c -fno-pie -o ntsc.o ntsc.c
	gcc -g -m32 ntsc.o -fno-pie -o dc
	rm ntsc.o

.PHONY: clean
clean:
	rm -rf ./*.o dc