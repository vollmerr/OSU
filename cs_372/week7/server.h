#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define CMD_LS      "-l"
#define CMD_GET     "-g"

#define MAX_DATA    256
#define MAX_CLIENTS 10

#define PORT        "9119" // TODO: CLI...

#define ERR_GET_ADRR    1
#define ERR_BIND        2
#define ERR_LISTEN      3
#define ERR_SELECT      4