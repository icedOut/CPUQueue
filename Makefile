mydu:
	gcc -Wall -g -std=c99 dispatch.c utils.c -o dispatch -lm

clean:
	rm -rf mydu *~ *.o