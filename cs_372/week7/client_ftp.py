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

class FTP:
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

    def __init__(self, host=None, port=None, port_data=None, cmd=None):
        # get the address info, use any passed from cli
        self.get_addr_info(host, port, port_data)
        # connect to the server
        self.connect()
        # send the server the data port
        self.cmd_port()
        # if they passed in a command, set it, otherwise get it when needed
        self.cmd = cmd
        # run the loop
        self.run()


    def __exit__(self, *args):
        self.print_debug('__exit__', args)


    def print_debug(self, *args):
        if self.debug:
            print(args)


    def print_error(self):
        print("Error: %s - %s" % (self.code, self.error))


    def get_addr_info(self, host, port, port_data):
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


    def connect(self):
        # make new file desc
        self.socket = socket.socket()
        # connect to server
        self.socket.connect((self.host, self.port))
        # TODO: ERROR HANDLING connect
        print('Connected to %s:%s', self.host, self.port)


    def recv(self):
        # get response from server
        self.data = self.socket.recv(MAX_DATA)
        # TODO: ERROR HANDLING recv
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
    

    def send(self, data=None):
        if data is None:
            self.socket.sendall(self.data)
        else:
            self.socket.sendall(data)
        # TODO: ERROR HANDLING sendall
        # get the response code from server
        return self.recv()


    def recv_all(self):
        pass


    def set_code(self, code=None):
        if code is None:
            self.code = self.data[:INDEX_CODE]
        else:
            self.code = code


    def set_size(self):
        print("SETTIG SIZE: ", self.data)
        try:
            self.size = int(self.data[INDEX_CODE:INDEX_SIZE])
        except:
            pass
        print("SIZE: %d" % self.size)

    
    def set_error(self):
        self.error = self.data[INDEX_CODE+1:]
        return self.print_error()

        
    def get_cmd(self):
        #  get command if none set (from cli params)
        if self.cmd is None:
            args = raw_input('Command:\t\t').split()
            if len(args) > 1:
                self.cmd, self.cmd_arg = args
            else:
                self.cmd, self.cmd_arg = [args[0], None]


    def send_cmd(self, cmd, data=None):
        if data is None:
            return self.send('%s' % cmd)
        return self.send('%s %s' % (cmd, data))


    def handle_cmd(self):
        c = self.cmd
        print("C::", c)
        if c == CMD_QUIT or c == '-q':
            return self.cmd_quit()
        elif c == CMD_PORT or c == '-p':
            return self.cmd_port()
        elif c == CMD_PWD or c == '-l':
            return self.cmd_pwd()
        elif c == CMD_RETR or c == '-f':
            return self.cmd_retr()
        else:
            pass


    def cmd_quit(self):
        if self.socket:
            self.send_cmd(CMD_QUIT)
            self.socket.close()
        self.close_data()
        # set code as quit to break run loop
        self.code = CMD_QUIT


    def cmd_port(self):
        return self.send_cmd(CMD_PORT, self.port_data)


    def cmd_pwd(self):
        if self.send_cmd(CMD_PWD):
            # get the dir listing from server on newtcp conneciton
            data = self.get_data()
            # print it out to stdout
            print(data)


    def cmd_retr(self):
        if self.send_cmd(CMD_RETR, self.cmd_arg):
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
            print("File saved as %s" % file_name)


    def get_data(self):
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
            return "%s %s" % (CODE_FAIL_CONN, "Failed to connect to server.")
        # get data from server
        data = b''
        size = self.size
        while len(data) < size:
            packet = self.socket_server.recv(size - len(data))
            # did not recv all of it, error
            if not packet:
                data = "%s %s" % (CODE_INT_ERR, "Failed to recv all data.")
                break
            data += packet
        # clean up connections
        self.close_data()
        return data


    def status(self):
        if self.code in ERR_CODES:
            return False
        return True


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


    def close_data(self):
        if self.socket_data:
            self.socket_data.close()
        if self.socket_server:
            self.socket_server.close()
