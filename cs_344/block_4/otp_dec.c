//////////////////////////////////////////////////////////////
//  file:     otp_enc.c
//  author:   Ryan Vollmer
//  purpose:  Performs one time pad style decryption on encrypted text
//  usage:    otp_dec <text> <key> <port>
//////////////////////////////////////////////////////////////
/*
#include "headers.h"

int main(int argc, char *argv[]) {
  int socketFD, charsWritten, charsRead;
  char buffer[2 * BIG_ENOUGH] = {0}, text[BIG_ENOUGH] = {0}, key[BIG_ENOUGH] = {0};
  char cur;
  FILE* fp = NULL;
  int i;
  char msg_len[64];
  // check argument length
  if (argc < 4) { print_usage(argv[ARG_PROG], "<textfile> <keyfile> <port>"); }   // Check usage & args
  // initialize socket
  socketFD = client_init(argv[ARG_PORT]);
  // Clear out the buffer arrays
  memset(buffer, '\0', sizeof(buffer));
  memset(text, '\0', sizeof(text));
  memset(key, '\0', sizeof(key));
  // read message file
  fp = fopen(argv[ARG_MSG], "r");
  if (fp) {
    i = 0; // buffer index
    while((cur = getc(fp)) != EOF) {
      // add all of message to buffer
      text[i] = cur;
      i++;
    }
    // remove newline char
    text[i - 1] = '\0';
    fclose(fp);
  }
// printf("MSG FILE OPENED: %s\n", buffer);
// read key file
  fp = fopen(argv[ARG_KEY], "r");
  if (fp) {
    i = 0; // buffer index
// printf("FILED OPENED!\n");
    while((cur = getc(fp)) != EOF) {
// printf("CURR: i:%d, cur: %c\n", i , cur);
      // add all of message to buffer
      key[i] = cur;
      i++;
    }
    // remove newline char
    key[i - 1] = '\0';
    fclose(fp);
  }
// printf("KEY FILE OPENED: %s\n", buffer);

  // validate length and correctness of message/key
  valid_length(text, key, argv[ARG_KEY]);
  valid_input(key, argv[ARG_KEY]);
  valid_input(text, argv[ARG_MSG]);

  // add message and key
  strcat(buffer, text);
  strcat(buffer, "##"); // END OF TEXT
  strcat(buffer, key);
  strcat(buffer, "@@"); // add END OF KEY
// printf("ENCRTYPED MSG TO SEND: %s", buffer);
// printf("WRITING TO SERVER: %s\n", buffer);
  fflush(stdout);
  // get message length to string
  memset(msg_len, '\0', sizeof(msg_len));
  // char tmp[64];
  // strcat(msg_len, itoa(strlen(buffer), tmp, 10));
  itoa(strlen(buffer), msg_len, 10);
  // send message length
  charsWritten = send(socketFD, msg_len, 64, 0);   // Write to the server
  if (charsWritten < 0) {
    errno = ERR_SEND;
    close(socketFD);
    print_err("Failed to send len to host");
  }
  // Send message to server
  charsWritten = send(socketFD, buffer, strlen(buffer), 0);   // Write to the server
  if (charsWritten < 0) {
    errno = ERR_SEND;
    close(socketFD);
    print_err("Failed to send msg to host");
  }

// printf("CHARS WRITTEN TO SERVER: %d\n", charsWritten);
// get return message length from server
  memset(msg_len, '\0', sizeof(msg_len));
  charsRead = recv(socketFD, msg_len, sizeof(msg_len), MSG_WAITALL);
  if (charsRead < 0) {
    errno = ERR_RECV;
    close(socketFD);
    print_err("Failed to read from host");
  }
  // Get return message from server
  memset(buffer, '\0', sizeof(buffer));   // Clear out the buffer again for reuse
  charsRead = recv(socketFD, buffer, atoi(msg_len), MSG_WAITALL);   // Read data from the socket, leaving \0 at end
  if (charsRead < 0) {
    errno = ERR_RECV;
    close(socketFD);
    print_err("Failed to read from host");
  }
  // print encrypted message
  printf("%s\n", buffer);
  // buffer[msg_len] = '\n';
  // write(STDOUT_FILENO, buffer, charsRead);
  fflush(stdout);
  // Close the socket
  close(socketFD);
  return EXIT_SUCCESS;
}*/
