import os
import sys
import socket

MAX_DATA = 1024
INDEX_CODE = 3
INDEX_SIZE = 13

CMD_PORT = "PORT"
CMD_PWD = "PWD"
CMD_RETR = "RETR"
CMD_QUIT = "QUIT"
CMD_HELP = "HELP"

CODE_OK = "200"
CODE_BAD_REQ = "400"
CODE_NOT_FOUND = "404"
CODE_FAIL_CONN = "499"
CODE_INT_ERR = "500"

ERR_CODES = { 
    CODE_BAD_REQ,
    CODE_NOT_FOUND,
    CODE_FAIL_CONN, 
    CODE_INT_ERR, 
    None
}

# clear screen and move currsor to top left
def clear():
    print('\33[2J\33[;f')


class FTP:
    """Initiates annd runs a mini-FTP client

    Commands:
        SHORT   LONG    ARG         DESC

        -p      PORT    portnum     sets the data port number     
        -l      PWD                 prints the working directory of the server
        -g      RETR    filename    retrieves a copy of file
        -q      QUIT                quits the client
        -h      HELP                prints this list of commands

    """
    debug = False

    cmd = None
    cmd_arg = None
    data = None
    code = None
    error = None
    size = None

    host = None
    port = None
    port_data = None
    socket = None
    socket_data = None 
    socket_server = None   

    # Initalies the ftp client, asks user for args if not provided
    # Connects to the sever, Sends the data port then
    def __init__(self, name=None, port=None, data=None, cmd=None, arg=None):
        # get the address info, use any passed from cli
        self.get_addr_info(name, port, data)
        # connect to the server
        self.connect()
        # send the server the data port
        self.cmd_port()
        # if they passed in a command, set it, otherwise get it when needed
        self.cmd = cmd
        self.cmd_arg = arg
        

    # do some cleanup on exit
    def __exit__(self, *args):
        self.print_debug('__exit__', args)
        self.close_data()
        self.close()


    # print messages only in debug mode
    def print_debug(self, *args):
        if self.debug:
            print(args)


    # print error messages with code and mesage
    def print_error(self):
        print("Error: %s - %s\n" % (self.code, self.error))


    # initalize host, port, and data, asking for any missing values
    def get_addr_info(self, host=None, port=None, port_data=None):
        clear()
        # set host from cli if none
        if host is None:
            self.host = raw_input('Host:\t\t\t')
        else:
            self.host = str(host)
        # set port for commands from cli if none
        if port is None:
            self.port = int(raw_input('Connection Port:\t'))
        else:
            self.port = int(port)
        # set port for data from cli if none
        if port_data is None:
            self.port_data = raw_input('Data Port:\t\t')
        else:
            self.port_data = str(port_data)

        self.print_debug('get_addr_info:\n\thost: %s\n\tport: %d\n\tport_data: %s',
            self.host, self.port, self.port_data)


    # connects to the server
    def connect(self):
        try:
            # make new file desc
            self.socket = socket.socket()
            # connect to server
            self.socket.connect((self.host, self.port))
            print('Connected to %s:%s\n' % (self.host, self.port))
        except Exception as e:
            print('Error: %s\n', e)
            raise


    # recieves the response from a command
    def recv(self):
        try:
            # get response from server
            self.data = self.socket.recv(MAX_DATA)
            # set the code
            self.set_code()
            # check the code
            ok = self.status()
            # set size if good
            if ok:
                self.set_size()
            # else set error
            else:
                self.set_error()
            return ok
        except Exception as e:
            print('Error: %s\n', e)
            return False
    

    # sends all the data to the server
    def send(self, data=None):
        try:
            # send the data
            if data is None:
                self.socket.sendall(self.data)
            else:
                self.socket.sendall(data)
            # get the response code from server
            return self.recv()
        except Exception as e:
            print('Error: %s\n', e)
            return False


    # sets the code from cmd responses
    def set_code(self, code=None):
        if code is None:
            self.code = self.data[:INDEX_CODE]
        else:
            self.code = code


    # sets the size of the data requested from cmd responses
    def set_size(self):
        try:
            self.size = int(self.data[INDEX_CODE:INDEX_SIZE])
        except:
            pass

    
    # sets the error message from cmd responses
    def set_error(self):
        self.error = self.data[INDEX_CODE+1:]
        return self.print_error()

        
    # gets a command from the user
    def get_cmd(self):
        #  get command if none set (from cli params)
        if self.cmd is None:
            try:
                args = raw_input('Command:\t').split()
                if len(args) > 1:
                    self.cmd, self.cmd_arg = args
                else:
                    self.cmd, self.cmd_arg = [args[0], None]
                self.cmd = self.cmd.upper()
            except:
                self.cmd = None


    # sends the command to the server
    def send_cmd(self, cmd, data=None):
        if data is None:
            return self.send('%s' % cmd)
        return self.send('%s %s' % (cmd, data))


    # calls the correct command based off user input
    def handle_cmd(self):
        c = self.cmd
        if c == CMD_QUIT or c == '-Q`':
            return self.cmd_quit()
        elif c == CMD_PORT or c == '-P':
            return self.cmd_port()
        elif c == CMD_PWD or c == '-L':
            return self.cmd_pwd()
        elif c == CMD_RETR or c == '-G':
            return self.cmd_retr()
        elif c == CMD_HELP or c == '-H':
            return self.cmd_help()
        else:
            print('Invalid command.\n')

    # stops the ftp client and closes any connections
    def cmd_quit(self):
        if self.socket:
            self.send_cmd(CMD_QUIT)
            self.socket.close()
        self.close_data()
        # set code as quit to break run loop
        self.code = CMD_QUIT


    # sends the data pot number to the server
    def cmd_port(self):
        return self.send_cmd(CMD_PORT, self.port_data)


    # prints the directory listing on the server
    def cmd_pwd(self):
        if self.send_cmd(CMD_PWD):
            # get the dir listing from server on newtcp conneciton
            data = self.get_data()
            # print it out to stdout
            print(data)


    # retrieves a copy of a file from the server
    def cmd_retr(self):
        if self.send_cmd(CMD_RETR, self.cmd_arg):
            try:
                # get the file from server on newtcp conneciton
                data = self.get_data()
                # get pieces of file name
                id = 0
                uid = ''
                fn = self.cmd_arg.split('.')
                name = fn[0]
                if len(fn) > 1:
                    dot = '.'
                    ext = fn[1]
                else:
                    dot = ''
                    ext = ''
                # while file exists, get unique name
                while (os.path.isfile('./' + name + uid + dot + ext)):
                    id = id + 1
                    uid = '_' + str(id)
                file_name = './' + name + uid + dot + ext
                # open file and write data to it
                with open(file_name, 'w') as f:
                    f.write(data)
                print("File saved as %s\n" % file_name)
            except Exception as e:
                print('Error: %s\n', e)
                return False

    # prints the help menu
    def cmd_help(self):
        menu = """
        Commands:
        \tSHORT   LONG    ARG         DESC

        \t-p      PORT    portnum     sets the data port number     
        \t-l      PWD                 prints the working directory of the server
        \t-g      RETR    filename    retrieves a copy of file
        \t-q      QUIT                quits the client
        \t-h      HELP                prints this help menu
        """
        print(menu)


    # gets data for the server on a new socket
    def get_data(self):
        try:
            # make new file desc
            self.socket_data = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # allow port to be reused with no time waiting...
            self.socket_data.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            # bind socket
            self.socket_data.bind(('', int(self.port_data)))
            # listen for incoming connections
            self.socket_data.listen(1)
            # accept the connection from server
            self.socket_server, addr = self.socket_data.accept()
            # if failed to connect, print error
            if self.socket_server < 0:
                return "%s %s" % (CODE_FAIL_CONN, "Failed to connect to server.\n")
            # get data from server
            data = b''
            size = self.size
            while len(data) < size:
                packet = self.socket_server.recv(size - len(data))
                # did not recv all of it, error
                if not packet:
                    data = "%s %s" % (CODE_INT_ERR, "Failed to recv all data.\n")
                    break
                data += packet
            # clean up connections
            self.close_data()
            return data
        except Exception as e:
                print('Error: %s\n', e)
                return ''


    # returns if the code is an error code
    def status(self):
        if self.code in ERR_CODES:
            return False
        return True


    # runs the loop of getting user commands
    def run(self):
        # check status code, as cmd gets reset
        while self.code != CMD_QUIT:
            # get a command from user
            self.get_cmd()
            # send command to server
            self.handle_cmd()
            # bad response from server
            if not self.status():
                # if a data connection was left opened clean it up
                self.close_data()
            # reset command
            self.cmd = None


    # closes the data connection created
    def close_data(self):
        if self.socket_data:
            self.socket_data.close()
        if self.socket_server:
            self.socket_server.close()


    # closes the main client command socket
    def close(self):
        if self.socket:
            self.socket.close()
