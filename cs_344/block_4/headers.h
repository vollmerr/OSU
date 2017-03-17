#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <sys/select.h>

#define VALID_CHARS         "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define VALID_CHARS_LENGTH  27
#define MAX_CONN            5
#define HOST_NAME           "localhost"
#define BIG_ENOUGH          100000
#define SMALL_ENOUGH        64
#define MAX_LEN             1024

#define ERR_NO_HOST         2
#define ERR_SOCKET          3
#define ERR_CONNECT         4
#define ERR_SEND            5
#define ERR_RECV            6
#define ERR_BIND            7

#define ARG_PROG            0
#define ARG_MSG             1
#define ARG_KEY             2
#define ARG_PORT            3

// utilities
void print_err(const char *err);
void print_usage(const char* prog, const char* msg);
int char_to_i(const char c);
char char_from_i(const int c);
char* itoa (int value, char *result, int base);

// client functions
int client_init(const char* port);
void client_err(const char *err, int socket_desc);
void client_file_read(char *file, char *buffer, int socket_desc);
void client_send(char *buffer, int socket_desc);
// void client_recv(char *buffer, int socket_desc); // FIXME: dont work...
void client_valid_input(const char* msg, const char* name, int socket_desc);
void client_valid_length(const char* msg, const char* key, const char* name, int socket_desc);
