CFLAGS = -Wall -Werror 

all: build

build:
	gcc $(CFLAGS) launcher.c readInput.c writeOutput.c sendUDP.c receiveUDP.c queueOperations.c list.c threadCanceller.c -o s-talk -lpthread
	
clean:
	rm -f s-talk