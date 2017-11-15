// written in C
// NO sendfile or predfiened functions that make trivial
// short and long files
// .txt file (extra for binary)
// CITE SOURCES AS YOU GO!
// CHECK OUTPUT / INPUT FROM PDF

// validate cli params
// wait on PORTNUM for clients
// establish TCP control conneciton with client
// wait for commands
// recieve invalid command = send error
// init TCP data connection on DATA_PORT
// recieve -l = send dir to client
// recieve -g FILENAME
// validate
// send contents of file
// send error if error (not found...)
// close conneciton with client
// loop - wait on PORTNUM for clients (#2)

// bonus = cd command, username/password to access server, binary files

#include "server.h"

/**
 * Initalizes the server socket information
 * 
 * @param {addrinfo*} ai    - address info to fill out
 */
void init_server_info(struct addrinfo *ai) {
  int r;
  struct addrinfo hints;

  memset(&hints, 0, sizeof hints);
  // fill address info
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, PORT, &hints, ai) != 0) {
    fprintf(stderr, "Failed to get address info\n");
    exit(ERR_GET_ADDR);
  }
}

/**
 * Initalizes the server socket and binds it
 * 
 * @param {addrinfo*} ai    - address info to use for socket
 * 
 * @return {int} fd         - file desc of server socket
 */
int init_server_socket(struct addrinfo *ai) {
  int fd;
  struct addrinfo *p;
  int yes = 1;  // for setsockopt() SO_REUSEADDR  
  // go through all address infos in the linked list
  for (p = ai; p != NULL; p = p->ai_next) {
    // make a new file desc
    fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (fd < 0) {
      continue;
    }
    // remove address reuse delay
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    // try binding
    if (bind(fd, p->ai_addr, p->ai_addrlen) < 0) {
      close(fd);
      continue;
    }
    // have fd and is bound, can get out
    break;
  }
  // if we got to end of link list, it means we didn't get bound
  if (p == NULL) {
    fprintf(stderr, "Failed to bind\n");
    exit(ERR_BIND);
  }
  return fd;
}

/**
 * Initalizes the server socket
 * 
 * @param {int} server_fd     - file desc of server socket
 */
void init_server(int *server_fd) {
  struct addrinfo *ai;
  // init the server addr info
  init_server_info(ai);
  // create new file desc
  *server_fd = init_server_socket(ai);
  // free address info
  freeaddrinfo(ai);
  // listen for new connections
  if (listen(*server_fd) < 0) {
    perror("Failed to listen");
    exit(ERR_LISTEN);
  }
}

/**
 * Initalizes all file descriptor sets
 * 
 * @param {fd_set*} master    - master set of file desc
 * @param {fd_set*} read_fds  - read set of file desc
 * @param {int} server_fd     - file desc of server socket
 */
void init_fd_sets(fd_set *master, fd_set *read_fds, int server_fd) {
  // clear all fd sets
  FD_ZERO(master);
  FD_ZERO(&read_fds);
  // add server socket to master
  FD_SET(server_fd, &master);
}

/**
 * Handles new client connections
 * 
 * @param {fd_set*} master    - master set of file desc
 * @param {int*} max_fd       - highest fd in master set
 */
void handle_new_client(fd_set *master, int *max_fd) {
  int fd;
  int addr_len;
  struct sockaddr client_addr;
  char clientIP[INET_ADDRSTRLEN];
  // accept the new connection
  addr_len = sizeof(client_addr);
  if ((fd = accept(server_fd, &client_addr, &addr_len) > 0) {
    perror("Failed to accept connection");
    return;
  }
  // add new fd to master set
  FD_SET(fd, master);
  // increment max fd
  if (fd > *max_fd) {
    *max_fd = fd;
  }
  // print message indicating new connection
  printf(
    "New connection from %s on socket %d\n",
    inet_ntop(AF_INET, get_in_addr(&client_addr), clientIP, INET_ADDRSTRLEN),
    fd,
  );
}

/**
 * Handles receiving data from a client
 */
void handle_recv_client() {
  // TODOQ
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
  // loop through connections forever
  while (true) {
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
          handle_new_client(master, &max_fd);
        } else {
          // data coming from existing client
          handle_recv_client();
        }
      }
    }
  }
}

int main() {
  fd_set master;
  fd_set read_fds;
  int server_fd;

  init_server(&server_fd);
  init_fd_sets(&master, &read_fds, server_fd);
  server_loop(&master, &read_fds, server_fd);

  return 0;
}
