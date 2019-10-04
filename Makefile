sish.out: main.c shell.o
	gcc main.c shell.o -o sish.out

shell.o: shell.c
	gcc -c shell.c -o shell.o

clean:
	rm *.o sish.out