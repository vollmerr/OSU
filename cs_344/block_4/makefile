CFLAGS = -g -Wall
_clean = -rm *o
path = ./bin/

all: otp_dec_d otp_dec otp_enc_d otp_enc keygen clean

otp_dec_d: $(path)otp_dec_d.c util.o server.o
	gcc $(CFLAGS) -o $@ $^

otp_dec: $(path)otp_dec.c util.o client.o
	gcc $(CFLAGS) -o $@ $^

otp_enc_d: $(path)otp_enc_d.c util.o server.o
	gcc $(CFLAGS) -o $@ $^

otp_enc: $(path)otp_enc.c util.o client.o
	gcc $(CFLAGS) -o $@ $^

keygen: $(path)keygen.c util.o
	gcc $(CFLAGS) -o $@ $^

client.o: $(path)client.c
	gcc $(CFLAGS) -c -o $@ $<

server.o: $(path)server.c
	gcc $(CFLAGS) -c -o $@ $<

util.o: $(path)util.c
	gcc $(CFLAGS) -c -o $@ $<

clean :
	$(_clean)
