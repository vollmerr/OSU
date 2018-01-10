CFLAGS = -g -Wall
_clean = -rm *o

all : smallsh

smallsh : smallsh.c sig_handlers.o cmd.o child.o util.o
	gcc $(CFLAGS) -o $@ $^
	$(_clean)

sig_handlers.o : sig_handlers.c
	gcc $(CFLAGS) -c -o $@ $<

cmd.o : cmd.c
	gcc $(CFLAGS) -c -o $@ $<

child.o : child.c
	gcc $(CFLAGS) -c -o $@ $<

util.o : util.c
	gcc $(CFLAGS) -c -o $@ $<

memcheck : smallsh
	valgrind --tool=memcheck --leak-check=yes smallsh

clean :
	$(_clean)
	-rm smallsh
