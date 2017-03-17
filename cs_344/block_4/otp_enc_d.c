//////////////////////////////////////////////////////////////
//  file:     otp_enc_d.c
//  author:   Ryan Vollmer
//  purpose:  Daemon for encrypting messages. Works in conjunction
//              with otp_enc
//  usage:    otp_enc_d <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int handle_connections (int *socket_listen, struct sockaddr_in *address, socklen_t *addr_len,  int *socket_clients, fd_set *socket_fds) {
  int socket_new, socket_read, max_sd, socket_desc, selection;
  char buffer[2 * BIG_ENOUGH], msg[BIG_ENOUGH], key[BIG_ENOUGH];
  int i;
  char msg_len[64];
  // clear socket file desc set
  FD_ZERO(socket_fds);
  // add server socket to set
  FD_SET(*socket_listen, socket_fds);
  max_sd = *socket_listen;
  // add other connections
  for (i=0; i<MAX_CONN; i++) {
    socket_desc = socket_clients[i];
    // add to list if valid
    if (socket_desc) FD_SET(socket_desc, socket_fds);
    // need highest sd for select func
    if (socket_desc > max_sd) max_sd = socket_desc;
  }
  // wait for activity on socket
  selection = select(max_sd + 1, socket_fds, NULL, NULL, NULL);
  if (selection < 0) {
    perror("SELECTION FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return 1;
  }
  // incoming connection
  else if (FD_ISSET(*socket_listen, socket_fds)) {
    // Accept a connection, blocking if one is not available until one connects
    socket_new = accept(*socket_listen, (struct sockaddr *)address, addr_len); // Accept
    if (socket_new < 0) {
      perror("ACCEPT FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      return 1;
    }

    /*
       // send new connection messgae
       char* message = "accpeting socket \n";
           //send new connection greeting message
          if( send(socket_new, message, strlen(message), 0) != strlen(message) )
          {
               perror(NULL); return 1;
          }

          puts("Welcome message sent successfully\n");
     */
    //add new socket to array of sockets
    for (i=0; i<MAX_CONN; i++) {
      //if position is empty
      if(!socket_clients[i]) {
        socket_clients[i] = socket_new;
// printf("Adding to list of sockets as %d\n", i);
        break;
      }
    }
  }
  // else other connection
  for (i=0; i<MAX_CONN; i++) {
    socket_desc = socket_clients[i];
    // socket active
    if (FD_ISSET(socket_desc, socket_fds)) {
      // Get the message from the client and display it
      memset(buffer, '\0', sizeof(buffer));
      memset(msg, '\0', sizeof(msg));
      memset(key, '\0', sizeof(key));
      // char tmp[10000] = {0};
      // Read the client's message from the socket
      // do {
      memset(msg_len, '\0', sizeof(msg_len));
      socket_read = recv(socket_desc, msg_len, sizeof(msg_len), MSG_WAITALL);
      // printf("RECVIED SERV: %s\n", buffer);
      if (socket_read < 0) {
        perror(NULL);
        return 1;
      }
      // printf("\n\n\n\nABOUT TO REVIECE MSG OF LEN : %d\n\n\n\n", atoi(msg_len));
      socket_read = recv(socket_desc, buffer, atoi(msg_len), MSG_WAITALL);
      // printf("RECVIED SERV: %s\n", buffer);
      if (socket_read < 0) {
        perror(NULL);
        return 1;
      }
      // } while (!strchr(tmp, '\0'));

//printf("SERVER: recv: \"%s\"\n", buffer);
      fflush(stdout);

      // check if closing connection
      if (!socket_read) {
        // close socket
        close(socket_desc);
        // mark as reusable
        socket_clients[i] = 0;
      }
      else {
        // get message from buffer
        i = 0;
        while (buffer[i - 1] != '#' && buffer[i] != '#') {
          msg[i] = buffer[i];
          i++;
        }
        msg[i] = '\0';
        // printf("msg final is: %s\n\n\n", msg);
        // move past ## and get key
        i+=2;
        int j = 0;
        while (buffer[i - 1] != '@' && buffer[i] != '@') {
          key[j] = buffer[i];
          // printf("key[%d] = %c\n", j, key[j]);
          j++;
          i++;
        }
        key[j] = '\0';
        // printf("\n\nkey final is: %s\n\n\n", key);
        int len_msg = strlen(msg);
        // encrypt message (msg + key % 27)
        for (i=0; i<len_msg; i++) {
          buffer[i] = char_from_i((char_to_i(msg[i]) + char_to_i(key[i])) % 27);
// printf("encoding: buffer[%d]/[%d] : %c\n", i, len_msg, buffer[i]);
        }
        buffer[i] = '\0';
        // printf("msg is after encrypt: %s\n", buffer);
        fflush(stdout);
        // get message length to string
        memset(msg_len, '\0', sizeof(msg_len));
        // char tmp[64];
        // strcat(msg_len, itoa(strlen(buffer), tmp, 10));
        itoa(strlen(buffer), msg_len, 10);
        // send message length
        socket_read = send(socket_desc, msg_len, 64, 0);
        if (socket_read < 0) {
          perror(NULL);
          return 1;
        }
        // Send a Success message back to the client
        socket_read = send(socket_desc, buffer, strlen(buffer), 0);   // Send success back
        if (socket_read < 0) {
          perror(NULL);
          return 1;
        }
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) print_usage(argv[0], "<port>");

  int socket_listen, socket_clients[MAX_CONN], port;
  struct sockaddr_in address;
  socklen_t addr_len;
  int i;
  int opt = 1;
  // set of socket descriptors
  fd_set socket_fds;
  // init client sockets
  for (i=0; i<MAX_CONN; i++) {
    socket_clients[i] = 0;
  }
  // Set up the socket
  socket_listen = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
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
  memset((char *)&address, '\0', sizeof(address)); // Clear out the address struct
  port = atoi(argv[1]); // Get the port number, convert to an integer from a string
  address.sin_family = AF_INET; // Create a network-capable socket
  address.sin_port = htons(port); // Store the port number
  address.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process
  // Enable the socket to begin listening
  if (bind(socket_listen, (struct sockaddr *)&address, sizeof(address)) < 0) {// Connect socket to port
    errno = ERR_BIND;
    print_err("Failed to bind socket");
  }
  // Flip the socket on - it can now receive up to 5 connections
  listen(socket_listen, MAX_CONN);
  // printf("LISTENING ON PORT %d", port);
  // accept connections
  addr_len = sizeof(address); // Get the size of the address for the client that will connect
  // keep server open, getting new connections
  while (1) {
    handle_connections(&socket_listen, &address, &addr_len, socket_clients, &socket_fds);
  } // END WHILE

  return EXIT_SUCCESS;
}
