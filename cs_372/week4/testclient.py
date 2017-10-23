import socket
import thread
import sys

RECV_BUFFER = 500

def get_port():
    if len(sys.argv) > 1:
        return int(sys.argv[1])
    return int(raw_input('Listen on port: '))

s = socket.socket()
host = socket.gethostname()
port = get_port()

user = raw_input('USERNAME: ')

s.connect((host, port))
print 'Connected to', host

def recv_message(s):
    s.send('Connection established.')

    while True:
        msg = s.recv(RECV_BUFFER)
        if not msg:
            break
        print(msg)

    s.close()

thread.start_new_thread(recv_message ,(s,))

while True:
    msg = raw_input(user + ' > ')
    s.send(user + ' > ' + msg)
