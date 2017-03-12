- https://en.wikipedia.org/wiki/One-time_pad
- encrypt/decrypt
  - 26 upper chars and ' '
  - encrypt -> message + key % 27
  - decrypt -> ciphertext - key % 27
  - destroy after use
- 5 small programs
- must use network calls from class (send(), recv, etc)
!!! no open() the files, imagine over network !!!
- all errors output to stderr

otp_enc_d:
  - bg daemon
  - output err if network err (ports unavailable, etc)
    - but not crash/exit unless prog starting up (bind())
    - if bad input, recognize, report err, continue
    - out to stderr
  - encodes
  - listens on port/socket assigned when first ran
  - call accept() when connection made
    - to generate socket for actual connection
  - use separate process to handle rest of connection
    - must make sure communicating with otp_enc
    - receives text and key from otp_enc via communication socket (not original listening socket)
    - write back ciphertext to otp_enc
    - key passed in must be at least as big as text
  - up to 5 concurrent socket connections
  - encrypting only in child, written back
  - set 5 in beginning or fork each time
  - usage: otp_enc_d listening_port
    - ex (always ran in bg):  otp_enc_d 57171 &
  - localhost target ip/host

- compile script > compileall
- 5 files (below), 5 plaintext files numbered 1 through 5, grading script, other files of mine in zip.

otp_enc:
  - connects to otp_enc_d
  - performs one time pad style encryption
  - doesn't do encryption, otp_enc_d does
  - usage: otp_enc plaintext key port
  - recieves ciphertext from otp_enc_d -> out to stdout
    - $ otp_enc myplaintext mykey 57171
    - $ otp_enc myplaintext mykey 57171 > myciphertext
    - $ otp_enc myplaintext mykey 57171 > myciphertext &
  - key/text with bad chars or short key -> exit with err, value 1
  - char validation (or in entp_enc_d)
  - if cannot find port -> print err, exit 2
  - exit 0 on successful run
  - should NOT be able to connect to otp_dec_d
    - have programs reject each other
    - report rejection, terminate

otp_dec_d:
  - same as otp_enc_d except decrypts instead of encrypt
  - returns plaintext to otp_dec

otp_dec:
  - same as dec_enc but decrypt
  - not be able to connect to otp_enc_d, same reject

keygen:
  - generates a key of specified len
  - gen any of 27 chars using UNIX randomize methods
  - Do not create spaces every five characters, as has been historically done.
  - nothing fancy, rand() is fine
  - '\n' on end
  - errs to stderr
  - usage: keygen keylength
    - ex: keygen 256 > mykey
    - 257 long cuz '\n'
