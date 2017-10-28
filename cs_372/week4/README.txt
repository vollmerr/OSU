[//] # ( 
  FOR A BETTER VIWING EXPERIENCE:
    - rename me to README.md, OR
    - https://github.com/vollmerr/OSU/tree/master/cs_372/week4
)

# Project 1 - CS 372

## About
Author: Ryan Vollmer
Purpose: Simple chat system designed for two users (one server, one client)
Tested on: 128.193.54.182:9119 (flip2)

## Usage
### Running the project:
1. Build the client:
```
make client
```

2. On Host A start the server:
```
python ./server.py <PORT>
```
(prompted for port number if not supplied)

3. On Host B start a client:
```
./client <IP_ADDRESS> <PORT>
```

### Closing a connection
While connected, send the message:
```
\quit
```
(or press 'Ctrl + C')


## Features
### Baseline
* server built using python
* client build using C
* Max message character limit of 500 (including username on client)
* 

### Enchancements
* Threaded client and server, allowing users to send multiple subsequent messages at any time
* Input prompt stays on bottom line, recived messages get placed above
* Program becomes encapulated window / Command prompts stay on the top line of console, clearing the rest of the screen


## Known Issues
* must press enter on server after client sends '\quit' command
* When '\quit' command is issued from server, waits until client tries to send an additional message before actually disconnecting.
* Input turns 'invisible' when typing a message and recive one before sending. Invisible input still sent in send buffer to server. (threading / mutex allocation issue) (client and server)


## References
### Sockets
- python https://docs.python.org/2/library/socket.html
- C 
    - https://codereview.stackexchange.com/questions/13461/two-way-communication-in-tcp-server-client-implementation
    - https://github.com/nileshjagnik/c-chat-server-client
    - https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpclient.c
    - http://joegle.com/code/chat.html
- Ip address resolving https://tuxbalaji.wordpress.com/2012/11/01/how-to-get-ip-address-in-python/


### Threading
- mutex / locks https://stackoverflow.com/questions/14888027/mutex-lock-threads
- general usage in python 
    - http://linuxgazette.net/107/pai.html
    - https://www.codeday.top/2017/06/22/27231.html
    - https://docs.python.org/2/library/threading.html
    - https://www.python-course.eu/threads.php
- general usage in C https://users.cs.cf.ac.uk/Dave.Marshall/C/node32.html#SECTION003270000000000000000

### Window management
- ASCII codes for window alteration http://www.climagic.org/mirrors/VT100_Escape_Codes.html
- removing text on a line https://stackoverflow.com/questions/1508490/erase-the-current-printed-console-line
- custom printf functions in C http://mylifeforthecode.com/creating-a-custom-printf-function-in-c/