//////////////////////////////////////////////////////////////
//  file:     otp_dec_d.c
//  author:   Ryan Vollmer
//  purpose:  Daemon for descytping messages. Works in conjunction
//              with otp_dec
//  usage:    otp_dec_d <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int handle_connections (int *socket_listen, struct sockaddr_in *address, socklen_t *addr_len,  int *socket_clients, fd_set *socket_fds) {
  int socket_new, socket_read, max_sd, socket_desc, selection, len_msg;
  char buffer[2 * BIG_ENOUGH], msg[BIG_ENOUGH], key[BIG_ENOUGH];
  int i, j;
  char msg_len[SMALL_ENOUGH];
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
    perror("Error: Selection failed");
    return 1;
  }
  // incoming connection
  else if (FD_ISSET(*socket_listen, socket_fds)) {
    // Accept a connection, blocking if one is not available until one connects
    socket_new = accept(*socket_listen, (struct sockaddr *)address, addr_len); // Accept
    if (socket_new < 0) {
      perror("Error: Accept Failed");
      return 1;
    }
    //add new socket to array of sockets
    for (i=0; i<MAX_CONN; i++) {
      //if position is empty
      if(!socket_clients[i]) {
        socket_clients[i] = socket_new;
        break;
      }
    }
  }
  // else other connection
  for (i=0; i<MAX_CONN; i++) {
    socket_desc = socket_clients[i];
    // socket active
    if (FD_ISSET(socket_desc, socket_fds)) {
      // clear out buffers
      memset(buffer, '\0', sizeof(buffer));
      memset(msg, '\0', sizeof(msg));
      memset(key, '\0', sizeof(key));
      memset(msg_len, '\0', sizeof(msg_len));
      // Get the message length from the client
      socket_read = recv(socket_desc, msg_len, sizeof(msg_len), MSG_WAITALL);
      if (socket_read < 0) {
        perror(NULL);
        return 1;
      }
      // get message from client
      socket_read = recv(socket_desc, buffer, atoi(msg_len), MSG_WAITALL);
      if (socket_read < 0) {
        perror(NULL);
        return 1;
      }
      fflush(stdout);
      // check if closing connection
      if (!socket_read) {
        // close socket
        close(socket_desc);
        // mark as reusable
        socket_clients[i] = 0;
      }
      else {
        i = 0;
        // get message from buffer
        while (buffer[i - 1] != '#' && buffer[i] != '#') {
          msg[i] = buffer[i];
          i++;
        }
        msg[i] = '\0';
        // printf("msg is: %s\n\n\n\n\n", msg);
        // move past "##"
        i+=2;
        j = 0;
        // get key from buffer
        while (buffer[i - 1] != '@' && buffer[i] != '@') {
          key[j] = buffer[i];
          j++;
          i++;
        }
        key[j] = '\0';
        len_msg = strlen(msg);
        j = 0;
        // decrypt message (msg - key % 27) + 27 if < 0
        for (i=0; i<len_msg; i++) {
          j = char_to_i(msg[i]) - char_to_i(key[i]);
          buffer[i] = char_from_i(j < 0 ? j + 27 : j % 27);
        }
        buffer[i] = '\0';
        fflush(stdout);
        // send message to client
        if (server_send(buffer, socket_desc)) return 1;
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) print_usage(argv[0], "<port>");
  int socket_listen, socket_clients[MAX_CONN];
  struct sockaddr_in address;
  socklen_t addr_len;
  // set of socket descriptors
  fd_set socket_fds;
  socket_listen = server_init(socket_clients, argv[1], &address, &addr_len);
  // keep server open, getting new connections
  while (1) {
    handle_connections(&socket_listen, &address, &addr_len, socket_clients, &socket_fds);
  } // END WHILE
  return EXIT_SUCCESS;
}
