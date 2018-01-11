'''

client.py

Ryan Vollmer
CS 372
Project 2

Basic FTP client.


usage: client.py [-h] [-n NAME] [-p PORT] [-d DATA] [-c CMD] [-a ARG]


optional arguments:
  -h, --help  show help message and exit
  -n NAME     server hostname or IP address
  -p PORT     server port number
  -d DATA     data port number
  -c CMD      command to use
  -a ARG      argument for command


Commands:
    SHORT   LONG    ARG         DESC

    -p      PORT    portnum     sets the data port number     
    -l      PWD                 prints the working directory of the server
    -g      RETR    filename    retrieves a copy of file
    -q      QUIT                quits the client
    -h      HELP                prints this list of commands

'''
import argparse
from client_ftp import FTP


# gets command line args to init ftp client
def get_args():
    parser = argparse.ArgumentParser(description='Basic FTP client.')

    parser.add_argument('-n', dest='name', help='server hostname or IP address')
    parser.add_argument('-p', dest='port', help='server port number')
    parser.add_argument('-d', dest='data', help='data port number')
    parser.add_argument('-c', dest='cmd', help='command to use')
    parser.add_argument('-a', dest='arg', help='argument for command')

    args = parser.parse_args()
    
    return {
        'name': args.name,
        'port': args.port,
        'data': args.data,
        'cmd': args.cmd,
        'arg': args.arg,
    }


def main():
    ftp = FTP(**get_args())
    ftp.run()


if __name__ == '__main__':
    main()