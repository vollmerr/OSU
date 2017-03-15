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
  if (!serverHostInfo) print_error("ERROR: cannot find port", EXIT_NO_HOST);
  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);   // Copy in the address

  // Set up the socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0);   // Create the socket
  if (socketFD < 0) print_error("CLIENT: ERROR opening socket", 1);

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)   // Connect socket to address
    print_error("CLIENT: ERROR connecting", 1);

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
    print_error("ERROR: key too short\n", EXIT_FAILURE);
  }
}

// argv -> prog, text, key, port
int main(int argc, char *argv[]) {
  int socketFD, charsWritten, charsRead;
  char buffer[10000];
  char cur;
  FILE* fp = NULL;
  int i, j;

  if (argc < 4) { print_usage(argv[ARG_PROG], "<textfile> <keyfile> <port>"); }   // Check usage & args

  socketFD = client_init(argv[ARG_PORT]);

  // Clear out the buffer array
  memset(buffer, '\0', sizeof(buffer));

  i = 0; // buffer index
  // read message file
  fp = fopen(argv[ARG_TEXT], "r");
  if (fp) {
    while((cur = getc(fp)) != EOF){
      // add all of message to buffer
      buffer[i] = cur;
      i++;
    }
    // remove newline char
    buffer[i - 1] = '\0';
    fclose(fp);
  }
printf("MSG FILE OPENED: %s\n", buffer);
  j = strlen(buffer);  // word len
  strcat(buffer, "##"); // add END OF TEXT
printf("BUFFFFER::: %s", buffer);
  i++;
  // read key file
  fp = fopen(argv[ARG_KEY], "r");
  if (fp) {
printf("FILED OPENED!\n");
    while((cur = getc(fp)) != EOF && j){
printf("CURR: %c\n", cur);
      // add all of message to buffer
      buffer[i] = cur;
      i++;
      j--;
    }
    // remove newline char
    buffer[i - 1] = '\0';
    fclose(fp);
  }
printf("KEY FILE OPENED: %s\n", buffer);


  // add message and key
  // strcat(buffer, buffer_text);
  // strcat(buffer, "##"); // END OF TEXT
  // strcat(buffer, buffer_key);
  strcat(buffer, "@@"); // add END OF KEY
// printf("ENCRTYPED MSG TO SEND: %s", buffer);

  //TODO:CHANGE TO OPEN FILES INSTEAD OF ARGS
  // valid_length(argv[ARG_TEXT], argv[ARG_KEY]);
  // valid_input(argv[ARG_TEXT]);
  // valid_input(argv[ARG_KEY]);

  // Send message to server
  charsWritten = send(socketFD, buffer, strlen(buffer), 0);   // Write to the server
  if (charsWritten < 0) print_error("CLIENT: ERROR writing to socket", 1);
  if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");
printf("CHARS WRITTEN TO SERVER\n");
  // Get return message from server
  memset(buffer, '\0', sizeof(buffer));   // Clear out the buffer again for reuse
  charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);   // Read data from the socket, leaving \0 at end
  if (charsRead < 0) print_error("CLIENT: ERROR reading from socket", 1);
printf("ENCRYPTED MESSAGE:: %s\n", buffer);

  close(socketFD);   // Close the socket

  return EXIT_SUCCESS;
}
