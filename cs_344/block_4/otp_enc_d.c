//////////////////////////////////////////////////////////////
//  file:     otp_enc_d.c
//  author:   Ryan Vollmer
//  purpose:  Daemon for encrypting messages. Works in conjunction
//              with otp_enc
//  usage:    otp_enc_d <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int main(int argc, char *argv[]) {
  if (argc < 2) print_usage(argv[0], "<port>");

  int socket_listen, client_conn, port, status;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;
  char buffer[10000];



/*INIT*/

  // Set up the address struct for this process (the server)
	memset((char *)&server_addr, '\0', sizeof(server_addr)); // Clear out the address struct
	port = atoi(argv[1]); // Get the port number, convert to an integer from a string
	server_addr.sin_family = AF_INET; // Create a network-capable socket
	server_addr.sin_port = htons(port); // Store the port number
	server_addr.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

  // Set up the socket
	socket_listen = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socket_listen < 0) print_error("ERROR opening socket", 1);

  // Enable the socket to begin listening
	if (bind(socket_listen, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) // Connect socket to port
		print_error("ERROR on binding", 1);
	listen(socket_listen, MAX_CONN); // Flip the socket on - it can now receive up to 5 connections
  printf("LISTENING ON PORT %d\n", port);

/*END INIT*/

  pid_t child = -5;
  int child_status;
  child = fork();
  switch(child) {
  case -1:
    exit(1);
    break;
  case 0: // child
    // Accept a connection, blocking if one is not available until one connects
  	client_size = sizeof(client_addr); // Get the size of the address for the client that will connect
  	client_conn = accept(socket_listen, (struct sockaddr *)&client_addr, &client_size); // Accept
  	if (client_conn < 0) print_error("ERROR on accept", 1);

    // Get the message from the client and display it
    memset(buffer, '\0', sizeof(buffer));
    status = recv(client_conn, buffer, 255, 0); // Read the client's message from the socket
    if (status < 0) print_error("ERROR reading from socket", 1);
    printf("SERVER: I received this from the client: \"%s\"\n", buffer);

    char msg[1024] = {0};
    char key[1024] = {0};
    // get message from buffer
    int i = 0;
    while (buffer[i - 1] != '#' && buffer[i] != '#') {
      msg[i] = buffer[i];
      i++;
    }
    printf("msg final is: %s\n", msg);
    // move past ## and get key
    i += 2;
    int j = 0;
    while (buffer[i - 1] != '@' && buffer[i] != '@') {
      key[j] = buffer[i];
      j++;
      i++;
    }
    printf("key final is: %s\n", key);

    // encrypt message (msg + key % 27)
    for (i=0; i<strlen(msg); i++) {
      msg[i] = char_from_i((char_to_i(msg[i]) + char_to_i(key[i])) % 27);
    }
    printf("msg is after encrypt: %s\n", msg);

    // Send a Success message back to the client
    status = send(client_conn, msg, strlen(msg), 0); // Send success back
    if (status < 0) print_error("ERROR writing to socket", 1);
    close(client_conn); // Close the existing socket which is connected to the client
    printf("SERVER - CHILD DONE\n");
    break;
  default:
    waitpid(child, &child_status, 0);
    printf("SERVER - PARENT DONE WAITING\n");
    break;
  }

	close(socket_listen); // Close the listening socket
  return EXIT_SUCCESS;
}
