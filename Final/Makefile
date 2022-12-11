program: solver.o
	gcc -g -o program solver.o

solver.o:
	gcc -c solver.c -Wall -lm

run:
	./program $(ARGS)

clean:
	rm -f program *.o *.out


