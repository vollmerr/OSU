/**
 * server_init.c
 *
 * Initialization of the server
 */

#include "server.h"

/**
 * Initalizes the server socket information
 *
 * @param {addrinfo**} ai    - pointer to address info to fill out
 */
void init_server_info(struct addrinfo **ai, char *port) {
  struct addrinfo hints;
  int r;
  // fill address info
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((r = getaddrinfo(NULL, port, &hints, ai)) != 0) {
    fprintf(stderr, "Failed to get address info: %s\n", gai_strerror(r));
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
void init_server(int *server_fd, char *port) {
  struct addrinfo *ai = NULL;
  char host[MAX_HOST_NAME];
  // init the server addr info
  init_server_info(&ai, port);
  // create new file desc
  *server_fd = init_server_socket(ai);
  // free address info
  freeaddrinfo(ai);
  // listen for new connections
  if (listen(*server_fd, MAX_CLIENTS) < 0) {
    perror("Failed to listen");
    exit(ERR_LISTEN);
  }
  // indicate the server is listening...
  gethostname(host, MAX_HOST_NAME);
  printf("Listening at\t\t%s:%s\n\n", host, PORT);
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
  FD_ZERO(read_fds);
  // add server socket to master
  FD_SET(server_fd, master);
}