server: main.o server.o utils.o print_output.o grid_operations.o
	gcc -o server main.o server.o utils.o print_output.o grid_operations.o

main.o: main.c
	gcc -c main.c

server.o: server.c
	gcc -c server.c

utils.o: utils.c
	gcc -c utils.c

print_output.o: print_output.c
	gcc -c print_output.c

grid_operations.o: grid_operations.c
	gcc -c grid_operations.c

clean:
	rm -f *.o server