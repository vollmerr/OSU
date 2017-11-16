CFLAGS = -g -Wall
	
all:
	server

client: server.c server.h
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	-rm -f *.o server