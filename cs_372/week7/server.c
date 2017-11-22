/*******************************************************
 * 
 * server.c
 * 
 * Ryan Vollmer
 * CS 372
 * Project 2
 * 
 * Basic FTP server
 * 
 * usage: server PORT
 * 
 * Commands Accepted:
 *   SHORT   LONG    ARG         DESC
 *
 *   -p      PORT    portnum     sets the data port number     
 *   -l      PWD                 prints the working directory of the server
 *   -g      RETR    filename    retrieves a copy of file
 *   -q      QUIT                quits the client
 * 
 *******************************************************/
#include "server.h"

int main(int argc, char *argv[]) {
  fd_set master;
  fd_set read_fds;
  int server_fd;

  print_clear();

  if (argc != 2) {
    printf("Usage: %s PORT", argv[0]);
    exit(1);
  }

  init_server(&server_fd, argv[1]);
  init_fd_sets(&master, &read_fds, server_fd);
  server_loop(&master, &read_fds, server_fd);

  return EXIT_SUCCESS;
}
