//////////////////////////////////////////////////////////////
//  file:     otp_enc.c
//  author:   Ryan Vollmer
//  purpose:  Performs one time pad style encryption on text
//  usage:    otp_enc <text> <key> <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int main(int argc, char *argv[]) {

    int socketFD, portNumber, charsWritten, charsRead;
  	struct sockaddr_in serverAddress;
  	struct hostent* serverHostInfo;
  	char buffer[256];

  	if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

  	// Set up the server address struct
  	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
  	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
  	serverAddress.sin_family = AF_INET; // Create a network-capable socket
  	serverAddress.sin_port = htons(portNumber); // Store the port number
  	serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
  	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
  	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

  	// Set up the socket
  	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
  	if (socketFD < 0) error("CLIENT: ERROR opening socket");

  	// Connect to server
  	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
  		error("CLIENT: ERROR connecting");

  	// Get input message from user
  	printf("CLIENT: Enter text to send to the server, and then hit enter: ");
  	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
  	fgets(buffer, sizeof(buffer) - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0
  	buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds

  	// Send message to server
  	charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
  	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
  	if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

  	// Get return message from server
  	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
  	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
  	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
  	printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

  	close(socketFD); // Close the socket

  // TODO: check inputs
  // TODO: checks key/text for bad chars, or short key
  // TODO connect to otp_enc_d
  // TODO: err (fail connect) print err, exit 2
  // TODO: err if try conenct to otp_dec_d
  // TODO: output to stdin
  return EXIT_SUCCESS;
}
