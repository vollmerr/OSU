//////////////////////////////////////////////////////////////
//  file:     otp_enc.c
//  author:   Ryan Vollmer
//  purpose:  Performs one time pad style encryption on text
//  usage:    otp_enc <text> <key> <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

const int ARG_PROG  = 0;
const int ARG_TEXT  = 1;
const int ARG_KEY   = 2;
const int ARG_PORT  = 3;

/**
 * Initalizes client connection
 * @param  port [description]
 * @return      [description]
 */
int client_init(const char* port) {
  int socketFD, portNumber;
  struct sockaddr_in serverAddress;
  struct hostent* serverHostInfo;
  // Set up the server address struct
  memset((char*)&serverAddress, '\0', sizeof(serverAddress));   // Clear out the address struct
  portNumber = atoi(port);   // Get the port number, convert to an integer from a string
  serverAddress.sin_family = AF_INET;   // Create a network-capable socket
  serverAddress.sin_port = htons(portNumber);   // Store the port number
  serverHostInfo = gethostbyname(HOST_NAME);   // Convert the machine name into a special form of address
  if (!serverHostInfo) {
    errno = ERR_NO_HOST;
    print_err("Host not found");
  }
  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);   // Copy in the address
  // Set up the socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0);   // Create the socket
  if (socketFD < 0) {
    errno = ERR_SOCKET;
    print_err("Failed to create socket");
  }
  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {  // Connect socket to address
    errno = ERR_CONNECT;
    print_err("Cannot connect to host");
  }
  // reutrn descriptor for later use
  return socketFD;
}

/**
 * Checks if key is too short for msg
 * @param msg [description]
 * @param key [description]
 */
void valid_length(const char* msg, const char* key) {
  int msg_len = strlen(msg);
  int key_len = strlen(key);
  // compare key to message length
  if (msg_len > key_len) {
     errno = EXIT_FAILURE;
     print_err("Key too short");
  }
}


int socket_send(int sd, char *buffer, int len) {
    int end = 0;        // how many bytes we've sent
    int total = len; // how many we have left to send
    ssize_t n;
    while(end < len) {
        n = send(sd, buffer + end, total, 0);
        printf("otp_enc: 71: %zd : %s\n", n, buffer + end);
        if (n == -1) { return -1; }
        end += n;
        total -= n;
    }
    if (n < len) printf("CLIENT: WARNING: Not all data written to socket!\n");
    return 0;
}


int main(int argc, char *argv[]) {
  int socketFD, charsWritten, charsRead;
  char buffer[2 * BIG_ENOUGH], text[BIG_ENOUGH], key[BIG_ENOUGH];
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
  fp = fopen(argv[ARG_TEXT], "r");
  if (fp) {
    i = 0; // buffer index
    while((cur = getc(fp)) != EOF){
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
    while((cur = getc(fp)) != EOF){
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
  valid_length(text, key);
  valid_input(key);
  valid_input(text);

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
}
