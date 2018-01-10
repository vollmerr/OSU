CFLAGS = -g -Wall

all:
	client

client: client.c
	gcc $(CFLAGS) -o $@ $^ -pthread

.PHONY: clean
clean:
	-rm -f *.o client