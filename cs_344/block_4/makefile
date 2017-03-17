CFLAGS = -g -Wall
_clean = -rm *o

all: otp_enc_d otp_enc keygen clean

otp_enc_d: otp_enc_d.c util.o server.o
	gcc $(CFLAGS) -o $@ $^

otp_enc: otp_enc.c util.o client.o
	gcc $(CFLAGS) -o $@ $^

keygen: keygen.c util.o
	gcc $(CFLAGS) -o $@ $^

client.o: client.c
	gcc $(CFLAGS) -c -o $@ $<

server.o: server.c
	gcc $(CFLAGS) -c -o $@ $<

util.o: util.c
	gcc $(CFLAGS) -c -o $@ $<

clean :
	$(_clean)
