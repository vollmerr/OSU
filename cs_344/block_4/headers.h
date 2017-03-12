#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define VALID_CHARS         "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define VALID_CHARS_LENGTH  27
#define MAX_CONN            5

// utilities
void print_error(const char* err, int code);
void print_usage(const char* prog, const char* msg);
