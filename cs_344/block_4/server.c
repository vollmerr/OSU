#include "headers.h"

/**
 * Initalizes server
 * @param  socket_clients - array of client socket descriptors
 * @param  port           - port to use
 * @param  address        - address struct to fill
 * @param  addr_len       - length of address struct
 * @return                - socket descriptor of server
 */
int server_init(int *socket_clients, const char *port, struct sockaddr_in *address, socklen_t *addr_len) {
  int socket_listen;
  int opt = 1;
  int i;
  // init client sockets
  for (i=0; i<MAX_CONN; i++) {
    socket_clients[i] = 0;
  }
  // Set up the socket
  socket_listen = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_listen < 0) {
    errno = ERR_SOCKET;
    print_err("Failed to create socket");
  }
  //set socket to allow multiple connections
  if(setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
    errno = ERR_SOCKET;
    print_err("Failed to create socket");
  }
  // Set up the address struct for this process (the server)
  memset((char *)address, '\0', sizeof(*address)); // Clear out the address struct
  address->sin_family = AF_INET; // Create a network-capable socket
  address->sin_port = htons(atoi(port)); // Store the port number
  address->sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process
  // Enable the socket to begin listening
  if (bind(socket_listen, (struct sockaddr *)address, sizeof(*address)) < 0) {// Connect socket to port
    errno = ERR_BIND;
    print_err("Failed to bind socket");
  }
  // Flip the socket on - it can now receive up to 5 connections
  listen(socket_listen, MAX_CONN);
  // accept connections
  *addr_len = sizeof(*address); // Get the size of the address for the client that will connect
  return socket_listen;
}
