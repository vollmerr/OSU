#####################################################################
#
#   server.py
#   Ryan Vollmer - CS 372, Project 1
#
#   Simple server side of chat system using threads to allow 
#       user input and output at the same time.
#
#   Usage: python server.py [ port number ]
#
#####################################################################

import socket
import sys
import os
import threading

MSG_SIZE = 500
QUIT = '\quit'
HOST = socket.gethostbyname(socket.gethostname())
PORT = 9119

def print_top(msg):
    ''' Prints a message at the top the screen

        @param{string} msg  - message to print
    '''
    print('\33[2J\33[;f') # escape code to clear screen and move currsor to top left
    print(msg)


def init_server():
    ''' Initalizes the server socker

        @return {object} server_socket  - initalized socket to use for server
    '''
    server_socket = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM,
        0
    )

    # get port number
    if len(sys.argv) > 1:
        PORT = int(sys.argv[1])
    else:
        PORT = int(raw_input('Listen on port: '))

    # allow port to be reused with no time waiting...
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    print('Listening at %s:%s' % (HOST, PORT))
  
    return server_socket


def get_connection(server_socket):
    ''' Sets up a connection to a client

        @param {object} server_socket   - server socket to connect to
        @return {object} client_socket  - client socket connected to server
    '''
    print_top('Listening at %s:%s' % (HOST, PORT))
    client_socket, client_addr = server_socket.accept()

    if client_socket < 0:
        print_top('Error accepting connection.\n')
        sys.exit(1)

    print_top('Connection established with %s:%s\n' % (client_addr[0], client_addr[1]))
    return client_socket


def recv_message(client_socket):
    ''' Handles reciving messages from the client

        @param {object} client_socket   - client socket server is connected to
    '''
    # run while default thread property 'do_run' is True
    recv = threading.currentThread()
    while getattr(recv, "do_run", True):
        msg = client_socket.recv(MSG_SIZE)
        
        if not msg or msg == QUIT:
            print_top('Connection closed. Press Enter to wait for new connections.\n')
            break

        # write message over prompt then reprompt (to keep input on bottom)
        sys.stdout.write('\33[2K\r') # escape code for resetting line
        sys.stdout.flush()
        sys.stdout.write(msg)
        sys.stdout.flush()
        sys.stdout.write('\n> ')
        sys.stdout.flush()
    client_socket.close()


def handle_connection(client_socket):
    ''' Handles the server side of the connection to a client

        @param {object} client_socket   - client socket server is connected to
    '''
    # start new thread for reciving client responses
    recv = threading.Thread(target=recv_message, args=(client_socket,))
    recv.start()
    # send input from cli to client
    while getattr(recv, "do_run", True):
        try:
            msg = raw_input('> ')
            client_socket.send(msg)

            if msg == QUIT:
                print_top('Connection closed.\n')
                # stop the loop and client thread's loop
                recv.do_run = False
                recv.join()
                break
        except:
            break


def server_loop(server_socket):
    ''' Main loop of server to wait for clients to connect

        @param {object} server_socket   - server socket to connect to    
    '''
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

