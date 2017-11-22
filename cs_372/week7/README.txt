# Project 2 - CS 372
## About

Author: Ryan Vollmer

Desc: Basic FTP client / server, allows retrieving text files and printing 
the servers wroking directory. Server supports up to 10 clients at once.


## Usage
### Basic usage
Build the server:
```
make server
```

Run the server
```
./server PORTNUM
```

Run the client(s)
```
python ./client.py [-h] [-n NAME] [-p PORT] [-d DATA] [-c CMD] [-a ARG]
```
(You will be prompted for any missing inputs)


## Available Commands
SHORT   LONG    ARG         DESC

-p      PORT    portnum     sets the data port number     
-l      PWD                 prints the working directory of the server
-f      RETR    filename    retrieves a copy of file
-q      QUIT                quits the client
-h      HELP                prints the help menu


## Sources
### C / server:
- general network programming -> https://beej.us/guide/bgnet/output/html/multipage/index.html
- accept example -> https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/accept.htm
- netinet usage -> http://pubs.opengroup.org/onlinepubs/000095399/basedefs/netinet/in.h.html
- get host name -> https://stackoverflow.com/questions/504810/how-do-i-find-the-current-machines-full-hostname-in-c-hostname-and-domain-info
- multiple packer recv -> https://stackoverflow.com/questions/4336612/handling-multiple-recv-calls-and-all-possible-scenarios
- ls cmd -> https://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
- remove newlines -> https://stackoverflow.com/questions/25615916/removing-newline-from-fgets
- ftp client (file only) -> ftp://gaia.cs.umass.edu/pub/kurose/ftpserver.c
- get client address from fd -> https://stackoverflow.com/questions/20472072/c-socket-get-ip-address-from-filedescriptor-returned-from-accept
- get host name form ip address -> https://www.gta.ufrj.br/ensino/eel878/sockets/gethostbynameman.html

### Python / client:
- ftp client -> https://github.com/python/cpython/blob/master/Lib/ftplib.py
- socket info -> https://docs.python.org/3.4/howto/sockets.html
- more socket info -> https://docs.python.org/2/library/socket.html
- ftp sepc -> http://www.networksorcery.com/enp/protocol/ftp.htm
- ftp cmds -> https://en.wikipedia.org/wiki/List_of_FTP_commands
- arg parsing -> https://docs.python.org/2/library/argparse.html#module-argparse
- recv multiple packets -> https://stackoverflow.com/questions/17667903/python-socket-receive-large-amount-of-data