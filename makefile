.PHONY: clean

assn1: main.c list.c 
	gcc -g main.c list.c -o assn1

clean: 
	rm -f assn1