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
 * Helper function for printing debug messages
 *
 * @param {char*} format    - format for message to display
 * @param {vargs} ...       - args for format to render
 */
void print_debug(const char *format, ...) {
#ifdef __DEBUG__
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  printf("\n");
  va_end(args);
#endif
}

/**
 * Removes newline characters
 */
char *trim(char *s) {
  int i = strlen(s) - 1;
  if ((i > 0) && (s[i] == '\n')) {
    s[i] = '\0';
  }
  return s;
}

/**
 * Initalizes the server socket information
 *
 * @param {addrinfo**} ai    - pointer to address info to fill out
 */
void init_server_info(struct addrinfo **ai) {
  print_debug("init_server_info");

  struct addrinfo hints;
  int r;

  memset(&hints, 0, sizeof hints);
  // fill address info
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((r = getaddrinfo(NULL, PORT, &hints, ai)) != 0) {
    fprintf(stderr, "Failed to get address info: %s\n", gai_strerror(r));
    exit(ERR_GET_ADDR);
  }

  print_debug("/init_server_info");
}

/**
 * Initalizes the server socket and binds it
 *
 * @param {addrinfo*} ai    - address info to use for socket
 *
 * @return {int} fd         - file desc of server socket
 */
int init_server_socket(struct addrinfo *ai) {
  print_debug("init_server_socket");

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

  print_debug("/init_server_socket");

  return fd;
}

/**
 * Initalizes the server socket
 *
 * @param {int} server_fd     - file desc of server socket
 */
void init_server(int *server_fd) {
  print_debug("init_server");

  struct addrinfo *ai = NULL;
  char host[MAX_HOST_NAME];
  // init the server addr info
  init_server_info(&ai);
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

  print_debug("/init_server");
}

/**
 * Initalizes all file descriptor sets
 *
 * @param {fd_set*} master    - master set of file desc
 * @param {fd_set*} read_fds  - read set of file desc
 * @param {int} server_fd     - file desc of server socket
 */
void init_fd_sets(fd_set *master, fd_set *read_fds, int server_fd) {
  print_debug("init_fd_sets");

  // clear all fd sets
  FD_ZERO(master);
  FD_ZERO(read_fds);
  // add server socket to master
  FD_SET(server_fd, master);

  print_debug("/init_fd_sets");
}

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

void handle_cmd_ls() {
  print_debug("handle_cmd_ls");

  DIR *dp;
  struct dirent *ep;
  dp = opendir("./");

  // opened dir
  if (dp != NULL) {
    // put newline seperate list into buffer
    while (ep = readdir(dp)) {
      puts(ep->d_name);
    }
    // close dir
    (void)closedir(dp);
  } else {
    perror("Couldn't open the directory");
  }

  print_debug("/handle_cmd_ls");
}

void handle_cmd(char *cmd) {
  print_debug("handle_cmd");

  if (strstr(cmd, CMD_LS)) {
    handle_cmd_ls();
  } else {
    printf("DEFAULT");
  }

  print_debug("/handle_cmd");
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
  }

  handle_cmd(trim(cmd));

  print_debug("recieved '%s'", cmd);
  print_debug("/handle_recv_client");
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

int main() {
  fd_set master;
  fd_set read_fds;
  int server_fd;

  init_server(&server_fd);
  init_fd_sets(&master, &read_fds, server_fd);
  server_loop(&master, &read_fds, server_fd);

  return 0;
}
