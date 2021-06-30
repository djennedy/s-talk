// make file

FLAGS = -Wall -Werror

all: 
	build

build:
	gcc $(FLAGS) launcher.c readInput.c writeOutput.c sendUDP.c receiveUDP.c  -pthread -o s-talk

run: 
	build
	./s-Talk


valgrind:
	build
	valgrind --leak-check=full ./s-Talk


clean:
	rm -f s-Talk