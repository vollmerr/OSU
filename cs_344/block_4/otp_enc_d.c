//////////////////////////////////////////////////////////////
//  file:     otp_enc_d.c
//  author:   Ryan Vollmer
//  purpose:  Daemon for encrypting messages. Works in conjunction
//              with otp_enc
//  usage:    otp_enc_d <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int server_init () {
  // Set up the address struct for this process (the server)
	memset((char *)&server_addr, '\0', sizeof(server_addr)); // Clear out the address struct
	port = atoi(argv[1]); // Get the port number, convert to an integer from a string
	server_addr.sin_family = AF_INET; // Create a network-capable socket
	server_addr.sin_port = htons(port); // Store the port number
	server_addr.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

  // Set up the socket
	socket_listen = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socket_listen < 0) print_error("ERROR opening socket");

  // Enable the socket to begin listening
	if (bind(socket_listen, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) // Connect socket to port
		print_error("ERROR on binding", errno);
	listen(socket_listen, MAX_CONN); // Flip the socket on - it can now receive up to 5 connections

  // Accept a connection, blocking if one is not available until one connects
	client_size = sizeof(client_addr); // Get the size of the address for the client that will connect
	client_conn = accept(socket_listen, (struct sockaddr *)&client_addr, &client_size); // Accept
	if (client_conn < 0) print_error("ERROR on accept");
}

int main(int argc, char *argv[]) {
  switch(argc) {
  case 2://TODO: handle background... //TODO: default port?

    break;
  default:
    print_usage(argv[0], "<port>");
    break;
  }


  int socket_listen, client_conn, port, status;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;

server_init();
	// Get the message from the client and display it
	memset(buffer, '\0', 256);
	status = recv(client_conn, buffer, 255, 0); // Read the client's message from the socket
	if (status < 0) print_error("ERROR reading from socket");
	printf("SERVER: I received this from the client: \"%s\"\n", buffer);

	// Send a Success message back to the client
	status = send(client_conn, "I am the server, and I got your message", 39, 0); // Send success back
	if (status < 0) print_error("ERROR writing to socket");
	close(client_conn); // Close the existing socket which is connected to the client
	close(socket_listen); // Close the listening socket

  //TODO: errors -> correct one fro merrno? -> pass code to error...

  // TODO: check inputsd
  // TODO: init 5 ssockets
  // TODO: bind - if err exit, print but continue if bad input, etc
  // TODO: output to stdout
  // TODO: listen on port assigned from args
  // TODO: call accept when conection made, generate socket for actual connect
  // TODO: create new process
  // TODO: ake sure communicating with otp_enc
  // TODO: get text and key from otp_enc via socket
  // TODO: encrypt message
  // TODO: send back encypted to otp_enc
  // TODO: maintain limit of 5 sockets
  return EXIT_SUCCESS;
}
