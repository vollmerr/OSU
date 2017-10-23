import socket
import signal
import sys

RECV_BUFFER = 500
QUIT = '\quit'


# gets the port number from command line
def get_port():
    if len(sys.argv) > 1:
        return int(sys.argv[1])
    return int(raw_input('Listen on port: '))


# initalized the server socket
def init_server():
    server = socket.socket()
    host = socket.gethostname()
    port = get_port()
    server.bind((host, port))

    server.listen(1) 
    return server


# sets up an connection to a client
def get_connection(server):
    print '[Waiting for connection...]'
    client, addr = server.accept()

    print 'Got connection from', addr
    return client


# handles a connection between the server and client
def handle_connection(client):
    while True:
        print '[Waiting for response...]'

        try:
            # get data fom client and print it
            data = client.recv(RECV_BUFFER)
            if data == QUIT:
                break
            print data
            # get data from server and sent it to client
            q = raw_input('Enter something to this client: ')
            if q == QUIT:
                break
            client.send(q)
        except:
            break
    print 'Closing connection...'


# main loop of the server
def server_loop(s):
    client = None

    while True:
        # wait for new client to connect if none
        if client is None:
            client = get_connection(s)
        # connected to client, send messages back and forth
        else:
            handle_connection(client)
            client.close()
            client = None


def main():
    server = init_server()
    server_loop(server)


if __name__ == '__main__':
    main()

