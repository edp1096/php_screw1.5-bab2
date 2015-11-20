all: screw descrew

screw: screw.c zencode.c
	gcc -o screw screw.c zencode.c -lz

descrew: descrew.c zencode.c
	gcc -o descrew descrew.c zencode.c -lz

clean:
	/bin/rm *.o *.so screw
