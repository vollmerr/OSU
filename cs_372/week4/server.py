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


# handles making cusor be on new line
# https://stackoverflow.com/questions/2082387/reading-input-from-raw-input-without-having-the-prompt-overwritten-by-other-th
def blank_current_readline():
    # Next line said to be reasonably portable for various Unixes
    (rows,cols) = struct.unpack('hh', fcntl.ioctl(sys.stdout, termios.TIOCGWINSZ,'1234'))

    text_len = len(readline.get_line_buffer())+2

    # ANSI escape sequences (All VT100 except ESC[0G)
    sys.stdout.write('\x1b[2K')                         # Clear current line
    sys.stdout.write('\x1b[1A\x1b[2K'*(text_len/cols))  # Move cursor up and clear line
    sys.stdout.write('\x1b[0G')                         # Move to start of line


# gets the port number from command line
def get_port():
    if len(sys.argv) > 1:
        return int(sys.argv[1])
    return int(raw_input('Listen on port: '))


# initalized the server socket
def init_server():
    server_socket = socket.socket()
    host = socket.gethostname()
    port = get_port()

    server_socket.bind((host, port))
    server_socket.listen(1)

    return server_socket


# sets up an connection to a client
def get_connection(server_socket):
    print('Waiting for a connection...\n')
    client_socket, client_addr = server_socket.accept()

    print('Connection established with ', client_addr)
    return client_socket


# handles reciving a message from client
def recv_message(client_socket):
    client_socket.send('Connection established.\n')

    while True:
        msg = client_socket.recv(RECV_BUFFER)
        blank_current_readline()
        
        if not msg or msg == QUIT:
            print('Connection closed. Press Enter to wait for new connections.\n')
            break
            
        print(msg)
        sys.stdout.write('> ' + readline.get_line_buffer())
        sys.stdout.flush()

    client_socket.close()


# handles a connection between the server and client
def handle_connection(client_socket):
    thread.start_new_thread(recv_message ,(client_socket,))

    while True:
        try:
            msg = raw_input('> ')

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
        # connected to client, send messages back and forth
        else:
            handle_connection(client_socket)
            client_socket.close()
            client_socket = None


def main():
    server_socket = init_server()
    server_loop(server_socket)


if __name__ == '__main__':
    main()

