CFLAGS = -g -Wall
	
all:
	server

server: server.c server.h server_cmd.c server_core.c server_init.c server_util.c
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	-rm -f *.o server