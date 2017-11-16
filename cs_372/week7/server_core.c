/**
 * server_core.c
 *
 * Core functionality of server
 */

#include "server.h"

/**
 * Handles new client connections
 *
 * @param {fd_set*} master    - master set of file desc
 * @param {int} server_fd     - file desc of server socket
 * @param {int*} max_fd       - highest fd in master set
 */
void handle_new_client(fd_set *master, int server_fd, int *max_fd) {
  print_debug("handle_new_client");

  int fd;
  socklen_t addr_len;
  struct sockaddr client_addr;
  char clientIP[INET_ADDRSTRLEN];
  // accept the new connection
  addr_len = sizeof(client_addr);
  if ((fd = accept(server_fd, &client_addr, &addr_len)) < 0) {
    perror("Failed to accept connection");
  } else {
    // add new fd to master set
    FD_SET(fd, master);
    // increment max fd
    if (fd > *max_fd) {
      *max_fd = fd;
    }
    // print message indicating new connection
    printf("New connection from %s on fd %d\n",
           inet_ntop(AF_INET, &(((struct sockaddr_in *)&client_addr)->sin_addr),
                     clientIP, INET_ADDRSTRLEN),
           fd);
  }

  print_debug("/handle_new_client");
}

/**
 * Handles receiving data from a client
 * Assumes length of message will be under MAX_DATA size
 * due to only being a command sent.
 *
 * @param {fd_set*} master    - master set of file desc
 * @param {int*} fd           - client fd to get data from
 */
void handle_recv_client(fd_set *master, int fd) {
  print_debug("handle_recv_client");

  int r;
  char cmd[MAX_DATA];
  // get data from client
  if ((r = recv(fd, cmd, MAX_DATA, 0)) <= 0) {
    // connection has been closed
    if (!r) {
      printf("Connection closed on socket %d", fd);
    } else {
      perror("Failed ro receive from client");
    }
    // clean up connection fd list
    close(fd);
    FD_CLR(fd, master);
  } else {
    // handle the command user sent
    handle_cmd(trim(cmd), fd);
  }

  print_debug("recieved '%s'", cmd);
  print_debug("/handle_recv_client");
}

/**
 * Handles sending data to a client
 *
 * @param {int*} fd       - client fd to send to
 * @param {char*} msg     - message to send
 */
void handle_send_client(int fd, char *msg) {
  print_debug("handle_send_client");

  int sent = 0;
  int len = strlen(msg);
  // while not all sent
  while (sent <= len) {
    printf("sending packet %d / %d to fd %d\n", (sent / MAX_DATA) + 1,
           (len / MAX_DATA) + 1, fd);
    // send MAX_DATA worth of bytes
    send(fd, msg + sent, MAX_DATA, 0);
    // increment position to send next time
    sent += MAX_DATA;
  }
  
  print_debug("/handle_send_client");
}

/**
 * Main server loop
 *
 * @param {fd_set*} master    - master set of file desc
 * @param {fd_set*} read_fds  - read set of file desc
 * @param {int} server_fd     - file desc of server socket
 */
void server_loop(fd_set *master, fd_set *read_fds, int server_fd) {
  print_debug("server_loop");

  int i;
  int max_fd = server_fd;
  // loop through connections forever
  while (1) {
    // copy fd set
    *read_fds = *master;
    // select the fds and put into the appropriate fd set
    if (select(max_fd + 1, read_fds, NULL, NULL, NULL) < 0) {
      perror("Failed to select");
      exit(ERR_SELECT);
    }
    // go through all connections
    for (i = 0; i <= max_fd; i++) {
      // is waiting to read
      if (FD_ISSET(i, read_fds)) {
        // fd is server
        if (i == server_fd) {
          // new connection incoming
          handle_new_client(master, server_fd, &max_fd);
        } else {
          // data coming from existing client
          handle_recv_client(master, i);
        }
      }
    }
  }

  print_debug("/server_loop");
}