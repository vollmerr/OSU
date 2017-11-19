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
}

/**
 * Handles receiving data from a client
 * Assumes length of message will be under MAX_DATA size
 * due to only being a command sent.
 *
 * @param {fd_set*} master    - master set of file desc
 * @param {int*} fd           - client fd to get data from
 * @param {char*} port        - client data port
 */
void handle_recv_client(fd_set *master, int fd, char *port) {
  char cmd[MAX_DATA] = "";
  // get data from client
  if (recv(fd, cmd, MAX_DATA, 0) <= 0) {
    // set command as quit to close connection
    strcpy(cmd, CMD_QUIT);
  }
  printf("about to handle... cmd: %s\n", cmd);
  // handle the command user sent
  handle_cmd(master, fd, port, cmd);
}

/**
 * Handles sending data to a client
 *
 * @param {int*} fd       - client fd to send to
 * @param {char*} msg     - message to send
 */
void handle_send_client(int fd, char *msg) {
  int sent = 0;
  int len = strlen(msg);
  // while not all sent
  while (sent <= len) {
    printf("sending packet %d / %d to fd %d\n", (sent / MAX_DATA) + 1,
           (len / MAX_DATA) + 1, fd);
    // send MAX_DATA worth of bytes
    send(fd, msg + sent, MIN(len, MAX_DATA), 0);
    // increment position to send next time
    sent += MAX_DATA;
  }
}

void handle_send_code(int fd, char *code, char *desc) {
  char buffer[MAX_DATA];
  // concat code infront of desc
  strcpy(buffer, code);
  strcpy(buffer, " ");
  strcpy(buffer, desc);
  // send code reponse to client
  handle_send_client(fd, buffer);
}

void handle_send_data(int fd, char *port, char *msg) {
  int sockfd, numbytes;
  char buf[MAX_DATA];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo("flip2", port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }

  // loop through all the results and connect to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("client: connect");
      close(sockfd);
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    exit(2);
  }
  
  inet_ntop(p->ai_family, &(((struct sockaddr_in *)p)->sin_addr), s,
            sizeof s);
  printf("client: connecting to %s\n", s);

  freeaddrinfo(servinfo);  // all done with this structure

  // if ((numbytes = recv(sockfd, buf, MAX_DATA - 1, 0)) == -1) {
  //   perror("recv");
  //   exit(1);
  // }

  // buf[numbytes] = '\0';

  // printf("client: received '%s'\n", buf);

  // close(sockfd);


  
  // int client_fd;
  // int rv;
  // // struct addrinfo *p;
  // struct addrinfo hints, *client_info, *p;
  // memset(&hints, 0, sizeof hints);
  // hints.ai_family = AF_INET;
  // hints.ai_socktype = SOCK_STREAM;
  // // connect to clients data port
  // struct sockaddr_in addr;
  // socklen_t addr_size = sizeof(struct sockaddr_in);
  // getpeername(fd, (struct sockaddr *)&addr, &addr_size);
  // // TODO erro handle...
  // if ((rv = getaddrinfo(inet_ntoa(addr.sin_addr), port, &hints,
  //                       &client_info)) != 0) {
  //   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
  //   return;
  // }
  // printf("tring to connect to... '%s' on port '%s' .... ",
  //        inet_ntoa(addr.sin_addr), port);
  // // loop through all the results and connect to the first we can
  // for (p = client_info; p != NULL; p = p->ai_next) {
  //   if ((client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
  //       -1) {
  //     continue;
  //   }

  //   if (connect(client_fd, p->ai_addr, p->ai_addrlen) == -1) {
  //     close(client_fd);
  //     continue;
  //   }

  //   break;
  // }
  // if (p == NULL) {
  //   fprintf(stderr, "client: failed to connect\n");
  //   return;
  // }

  // freeaddrinfo(client_info);  // all done with this structure
  // // send data
  // handle_send_client(client_fd, msg);
  // // close connection
  // close(client_fd);

  // int client_port;
  // char *client_addr;
  // int client_fd;

  // // struct sockaddr_in name;
  // // socklen_t addr_size = sizeof(struct sockaddr_in);
  // // getpeername(fd, (struct sockaddr *)&name, &addr_size);

  // // client_addr = inet_ntoa(name.sin_addr);
  // // client_port = strtol(port, NULL, 10);
  // client_fd = socket(AF_INET, SOCK_STREAM, 0);
  // if (client_fd < 0) {
  //   printf("Error creating socket!\n");
  //   exit(1);
  // }

  // struct sockaddr_in addr;
  // memset(&addr, 0, sizeof(addr));
  // addr.sin_family = AF_INET;
  // addr.sin_addr.s_addr = inet_addr("flip2");
  // addr.sin_port = htons(9118);

  // // connect to server
  // if (connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
  //   printf("Error connecting to the server!\n");
  //   exit(1);
  // }
  // printf("Connected to \t\t%s:%d\n\n", client_addr, client_port);
  handle_send_client(sockfd, msg);
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
  int i;
  int max_fd = server_fd;
  char port[MAX_DATA] = "";
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
          handle_recv_client(master, i, port);
        }
      }
    }
  }
}