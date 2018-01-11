//////////////////////////////////////////////////////////////
//  file:     otp_enc.c
//  author:   Ryan Vollmer
//  purpose:  Performs one time pad style encryption on text
//  usage:    otp_enc <text> <key> <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int main(int argc, char *argv[]) {
  int socket_desc;
  char buffer[2 * BIG_ENOUGH] = {0}, msg[BIG_ENOUGH] = {0}, key[BIG_ENOUGH] = {0}, msg_len[SMALL_ENOUGH] = {0};
  // check argument length
  if (argc < 4) print_usage(argv[ARG_PROG], "<textfile> <keyfile> <port>");
  // initialize socket
  socket_desc = client_init(argv[ARG_PORT]);
  // read message / key files
  client_file_read(argv[ARG_MSG], msg, socket_desc);
  client_file_read(argv[ARG_KEY], key, socket_desc);
  // validate length and correctness of message/key
  client_valid_length(msg, key, argv[ARG_KEY], socket_desc);
  client_valid_input(key, argv[ARG_KEY], socket_desc);
  client_valid_input(msg, argv[ARG_MSG], socket_desc);
  // add message and key
  strcat(buffer, msg);
  strcat(buffer, "##"); // END OF TEXT
  strcat(buffer, key);
  strcat(buffer, "@@"); // add END OF KEY
  // send message to server
  client_send(buffer, socket_desc);
  // client_recv(buffer, socket_desc); // TODO: figure out why moving recv to function breaks it...
  // get size to recv
  if (recv(socket_desc, msg_len, sizeof(msg_len), MSG_WAITALL) < 0) {
    errno = ERR_RECV;
    close(socket_desc);
    print_err("Failed to read from host");
  }
  // Clear out the buffer again for reuse
  memset(buffer, '\0', sizeof(buffer));
  // get encypted messgae back
  if (recv(socket_desc, buffer, atoi(msg_len), MSG_WAITALL) < 0) {
    errno = ERR_RECV;
    close(socket_desc);
    print_err("Failed to read from host");
  }
  // print encrypted message
  printf("%s\n", buffer);
  fflush(stdout);
  // Close the socket
  close(socket_desc);
  return EXIT_SUCCESS;
}
