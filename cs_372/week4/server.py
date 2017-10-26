import socket
import signal
import sys
import thread
import struct
import fcntl
import termios
import readline

RECV_BUFFER = 500
QUIT = '\quit'
PORT = int(sys.argv[1])
HOST = socket.gethostbyname(socket.gethostname())


# initalizes the server socket
def init_server():
    server_socket = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM,
        0
    )

    # allow port to be reused with no time waiting...
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen(1)
    print('Lisening at %s:%s' % (HOST, PORT))
  
    return server_socket


# sets up an connection to a client
def get_connection(server_socket):
    print('Waiting for a connection...\n')
    client_socket, client_addr = server_socket.accept()

    if client_socket < 0:
        print('Error accepting connection.\n')
        sys.exit(1)

    print('Connection established with ', client_addr)
    return client_socket


# handles reciving a message from client
def recv_message(client_socket):
    while True:
        msg = client_socket.recv(RECV_BUFFER)
        
        if not msg or msg == QUIT:
            print('Connection closed. Press Enter to wait for new connections.\n')
            break

        print(msg)
    client_socket.close()


# handles a connection between the server and client
def handle_connection(client_socket):
    # new thread for reciving responses
    thread.start_new_thread(recv_message ,(client_socket,))
    # send input from cli to client
    while True:
        try:
            msg = raw_input('')

            if msg == QUIT:
                print('Connection closed.\n')
                break

            client_socket.send(msg)
        except:
            break


# main loop of the server
def server_loop(server_socket):
    client_socket = None

    while True:
        # wait for new client to connect if none
        if client_socket is None:
            client_socket = get_connection(server_socket)
        # if connected to client, send messages back and forth
        else:
            handle_connection(client_socket)
            client_socket.close()
            client_socket = None


def main():
    server_socket = init_server()
    server_loop(server_socket)


if __name__ == '__main__':
    main()

