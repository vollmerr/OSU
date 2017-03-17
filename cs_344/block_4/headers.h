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
#define MAX_LEN             1024

#define ERR_NO_HOST         2
#define ERR_SOCKET          3
#define ERR_CONNECT         4
#define ERR_SEND            5
#define ERR_RECV            6
#define ERR_BIND            7


// utilities
void print_err();
void print_usage(const char* prog, const char* msg);

void valid_input(const char* msg, const char* name);
void valid_length(const char* msg, const char* key, const char* name);
int char_to_i(const char c);
char char_from_i(const int c);
char* itoa (int value, char *result, int base);
