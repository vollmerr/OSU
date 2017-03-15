#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

#define VALID_CHARS         "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define VALID_CHARS_LENGTH  27
#define MAX_CONN            5
#define HOST_NAME           "localhost"

#define EXIT_NO_HOST        2

// utilities
void print_error(const char* err, int code);
void print_usage(const char* prog, const char* msg);

void valid_input(const char* msg);
int char_to_i(const char c);
char char_from_i(const int c);
