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
  print_debug("handle_new_client: '%d' - '%d'", server_fd, *max_fd);
  int fd;
  socklen_t addr_len;
  struct sockaddr client_addr;
  char clientIP[NI_MAXHOST];
  char clientPort[NI_MAXSERV];
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
    if (getnameinfo(&client_addr, addr_len, clientIP, INET_ADDRSTRLEN,
                    clientPort, INET_ADDRSTRLEN,
                    NI_NUMERICHOST | NI_NUMERICSERV)) {
      perror("Failed to client information");
    } else {
      printf("New connection from %s:%s on fd %d\n", clientIP, clientPort, fd);
    }
  }
}

/**
 * Handles receiving data from a client
 * Assumes length of message will be under MAX_DATA size
 * due to only being a command sent.
 *
 * @param {fd_set*} master        - master set of file desc
 * @param {int*} fd               - client fd to get data from
 * @param {char**} client_ports   - list of client data ports
 */
void handle_recv_client(fd_set *master, int fd, char **client_ports) {
  print_debug("handle_recv_client: '%d'", fd);
  char cmd[MAX_DATA] = "";
  // get data from client
  if (recv(fd, cmd, MAX_DATA, 0) <= 0) {
    // set command as quit to close connection
    strcpy(cmd, CMD_QUIT);
  }
  // handle the command user sent
  handle_cmd(master, fd, client_ports, cmd);
}

/**
 * Handles sending data to a client
 *
 * @param {int*} fd       - client fd to send to
 * @param {char*} msg     - message to send
 * @param {int} len       - length of data
 */
void handle_send_client(int fd, const char *msg, long int len) {
  print_debug("handle_send_client: '%d' - '%s' - '%ld'", fd, msg, len);
  long int sent = 0;
  // while not all sent
  while (sent <= len) {
    printf("Sent:\t\t%ld / %ld to fd %d\n", (sent / MAX_DATA) + 1,
           (len / MAX_DATA) + 1, fd);
    // send MAX_DATA worth of bytes
    send(fd, msg + sent, MIN(len, MAX_DATA), 0);
    // increment position to send next time
    sent += MAX_DATA;
  }
  printf("\n");
}

/**
 * Handles sending a code and message.
 * Message will be text on errors and
 * length of data on success.
 *
 * @param {int*} fd       - client fd to send to
 * @param {char*} code    - code to send
 * @param {char*} msg     - message to send
 */
void handle_send_code(int fd, const char *code, const char *msg) {
  print_debug("handle_send_code: '%d' - '%s' - '%s'", fd, code, msg);
  char buffer[MAX_DATA];
  // concat code infront of msg
  sprintf(buffer, "%s %s", code, msg);
  printf("Sending:\t'%s' to fd %d\n", buffer, fd);
  // send code reponse to client
  handle_send_client(fd, buffer, strlen(buffer));
}

/**
 * Handles sending data (string format) to
 * a client. Opens a new TCP connection, connecting
 * to the clients data port.
 *
 * @param {int*} fd       - client fd to send to
 * @param {char*} port    - clients port to connect to
 * @param {char*} msg     - message to send
 * @param {int} len       - length of message
 */
void handle_send_data(int fd, const char *port, const char *msg, long int len) {
  print_debug("handle_send_data: '%d' - '%s' - '%s' - %ld", fd, port, msg, len);
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET_ADDRSTRLEN];
  // will do for now as spent wayyy too many hours on this....
  // get ip address based off clients fd
  struct sockaddr_in addr;
  socklen_t addr_size = sizeof(struct sockaddr_in);
  getpeername(fd, (struct sockaddr *)&addr, &addr_size);
  char clientip[20];
  strcpy(clientip, inet_ntoa(addr.sin_addr));
  // get host name based off ip address.........
  struct hostent *he;
  struct in_addr hostname;
  inet_aton(clientip, &hostname);
  he = gethostbyaddr(&hostname, sizeof hostname, AF_INET);
  // fill socket info
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  // get address info
  if ((rv = getaddrinfo(he->h_name, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }
  // loop through all the results and connect to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("socket");
      continue;
    }
    // connnect to client
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("connect");
      close(sockfd);
      continue;
    }
    break;
  }
  // make sure we actually have a socket
  if (p == NULL) {
    fprintf(stderr, "Failed to connect to client\n");
    exit(2);
  }

  inet_ntop(p->ai_family, &(((struct sockaddr_in *)p)->sin_addr), s, sizeof s);
  printf("Connecting to %s:%s on fd %d\n", he->h_name, port, sockfd);
  // free our addr info
  freeaddrinfo(servinfo);
  // send the data to the client
  handle_send_client(sockfd, msg, len);
  close(sockfd);
}

/**
 * Main server loop
 *
 * @param {fd_set*} master    - master set of file desc
 * @param {fd_set*} read_fds  - read set of file desc
 * @param {int} server_fd     - file desc of server socket
 */
void server_loop(fd_set *master, fd_set *read_fds, int server_fd) {
  print_debug("server_loop: '%d'", server_fd);
  int i;
  int max_fd = server_fd;
  char *client_ports[MAX_CLIENTS];
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
          handle_recv_client(master, i, client_ports);
        }
      }
    }
  }
}