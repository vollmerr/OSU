build : vollmerr.buildrooms.c
	gcc -o vollmerr.buildrooms -g vollmerr.buildrooms.c; \
	valgrind ./vollmerr.buildrooms

play : vollmerr.adventure.c
	gcc -o vollmerr.adventure -g vollmerr.adventure.c -lpthread; \
	valgrind ./vollmerr.adventure

vplay : vollmerr.adventure.c
	vi vollmerr.adventure.c

clean :
	rm -r vollmerr.rooms.*; \
	ls
