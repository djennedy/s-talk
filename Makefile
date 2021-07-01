CFLAGS = -Wall -Werror

all: build

build:
	gcc $(CFLAGS) launcher.c readInput.c writeOutput.c sendUDP.c receiveUDP.c queueOperations.c  -pthread -o s-talk

run: build
	./s-talk


valgrind: build
	valgrind --leak-check=full ./s-talk


clean:
	rm -f s-talk