#include "headers.h"

/**
 * Prints errors for clients and closes their connection
 * @param err         - error to display
 * @param socket_desc - socket to close
 */
void client_err(const char *err, int socket_desc) {
  fprintf( stderr, "Error: %s\n", err);
  close(socket_desc);
  exit(errno);
}

/**
 * Initalizes client connection
 * @param  port  - port number
 * @return       - socket file descriptor
 */
int client_init(const char* port) {
  int socket_desc, socket_port;
  struct sockaddr_in socket_address;
  struct hostent* socket_host;
  // Set up the server address struct
  memset((char*)&socket_address, '\0', sizeof(socket_address));   // Clear out the address struct
  socket_port = atoi(port);   // Get the port number, convert to an integer from a string
  socket_address.sin_family = AF_INET;   // Create a network-capable socket
  socket_address.sin_port = htons(socket_port);   // Store the port number
  // get host info
  if (!(socket_host = gethostbyname(HOST_NAME))) {
    errno = ERR_NO_HOST;
    perror("Error: Host not found");
    exit(errno);
  }
  // Copy in the address
  memcpy((char*)&socket_address.sin_addr.s_addr, (char*)socket_host->h_addr, socket_host->h_length);
  // Set up the socket
  if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    errno = ERR_SOCKET;
    perror("Error: Failed to create socket");
    exit(errno);
  }
  // Connect to server
  if (connect(socket_desc, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0) {
    errno = ERR_CONNECT;
    client_err("Cannot connect to host", socket_desc);
  }
  // reutrn descriptor for later use
  return socket_desc;
}

/**
 * Reads file into buffer
 * @param file            - file being read
 * @param buffer          - buffer to read into
 * @param socket_desc     - socket to close on error
 */
void client_file_read(char *file, char *buffer, int socket_desc) {
  FILE *fp = NULL;
  char err[MAX_LEN];
  char cur;
  int i;
  // open file
  fp = fopen(file, "r");
  if (fp) {
    i = 0; // buffer index
    while((cur = getc(fp)) != EOF) {
      // add all of message to buffer
      buffer[i] = cur;
      i++;
    }
    // remove newline char
    buffer[i - 1] = '\0';
    fclose(fp);
  }
  // failed to open file
  else {
    errno = EXIT_FAILURE;
    sprintf(err, "Failed to open file %s", file);
    client_err(err, socket_desc);
  }
}

////////////////////////////////////////////////////////////////////////////////
//  Socket interaction
////////////////////////////////////////////////////////////////////////////////
/**
 * Sends message to host
 * @param buffer      - buffer to send containing message
 * @param socket_desc - socket to close on error
 */
void client_send(char *buffer, int socket_desc) {
  char msg_len[SMALL_ENOUGH] = {0};
  // make message length a string
  itoa(strlen(buffer), msg_len, 10);
  // send message length to server
  if (send(socket_desc, msg_len, SMALL_ENOUGH, 0) < 0) {
    errno = ERR_SEND;
    client_err("Failed to send message length to host", socket_desc);
  }
  // Send message to server
 if (send(socket_desc, buffer, strlen(buffer), 0) < 0) {
    errno = ERR_SEND;
    client_err("Failed to send message to host", socket_desc);
  }
}
/**
 * FIXME: CURRENTLY DOESNT WORK - figure out why moving to functino breaks it
 * Recieves message from host
 * @param buffer      - buffer to put message into
 * @param socket_desc - socket to close on error
 */
void client_recv(char *buffer, int socket_desc) {
  char msg_len[SMALL_ENOUGH] = {0};
  // get return message length from server
  if (recv(socket_desc, msg_len, SMALL_ENOUGH, MSG_WAITALL) < 0) {
    errno = ERR_RECV;
    client_err("Failed to read message length from host", socket_desc);
  }
  // Get return message from server
  memset(buffer, '\0', sizeof(buffer));
  if (recv(socket_desc, buffer, atoi(msg_len), MSG_WAITALL) < 0) {
    errno = ERR_RECV;
    client_err("Failed to read message from host", socket_desc);
  }
}

////////////////////////////////////////////////////////////////////////////////
//  Validation
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if input is valid charaters
 * @param msg         - message to check if valid
 * @param name        - name of file provided
 * @param socket_desc - socket descriptor to close on error
 */
void client_valid_input(const char* msg, const char* name, int socket_desc) {
  int i = 0;
  // check all chars are valid
  while (msg[i]) {
    if (!strchr(VALID_CHARS, msg[i])) {
      char err[MAX_LEN];
      errno = EXIT_FAILURE;
      sprintf(err, "Invalid characters in %s", name);
      client_err(err, socket_desc);
    }
    i++;
  }
}
/**
 * Checks if input/key is valid length
 * @param msg         - message to check if valid against
 * @param key         - key to check if long enough
 * @param name        - name of key file provided
 * @param socket_desc - socket descriptor to close on error
 */
void client_valid_length(const char* msg, const char* key, const char* name, int socket_desc) {
  // compare key to message length
  if (strlen(msg) > strlen(key)) {
    char err[MAX_LEN];
    errno = EXIT_FAILURE;
    sprintf(err, "Key %s too short", name);
    client_err(err, socket_desc);
  }
}
