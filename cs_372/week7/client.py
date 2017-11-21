import argparse
from client_ftp import FTP


# gets command line args to init ftp client
def get_args():
    parser = argparse.ArgumentParser(description='Basic FTP client.')

    parser.add_argument('-a', dest='addr', help='server hostname or IP address')
    parser.add_argument('-p', dest='port', help='server port number')
    parser.add_argument('-d', dest='data', help='data port number')
    parser.add_argument('cmd', nargs='?', help='command to use')

    args = parser.parse_args()
    
    return {
        'addr': args.addr,
        'port': args.port,
        'data': args.data,
        'cmd': args.cmd,
    }


def main():
    ftp = FTP(**get_args())
    ftp.run()


if __name__ == '__main__':
    main()