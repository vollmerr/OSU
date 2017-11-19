void init_server_socket() {
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

void init_server_info() {
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
}

void init_server() {
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
}

void run_server() {
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
}

int main() {
    init_server();
    run_server();

// server loop - wait for conenctions
    // accept connection with client
    // fork connection

        // while command is not quit
            // get command from client
                // parse space seperated command in format "CMD ARG"
                // if no data, set command as QUIT

            // handle command
                // call correct command

                // PORT - set data port
                // set data port to arg
                // bad or no arg, send bad response
                // send ok

                // PWD - print working directory
                // get current dir listing
                // port not set, send data port not set
                // send ok
                // init data connection on data port
                // send data
                // close data connection   

                // RETR - retrieve a file
                // get file
                // not found, send file not found
                // port not set, send data port not set
                // send ok
                // init data connection on data port
                // send data
                    // get response
                    // try again if fail
                // close data connection    

                // QUIT - close the connection
                // close the connection

                // unknown command
                // send bad command

        // close client socket
        // close fork

// close server socket fd

return 0;
}