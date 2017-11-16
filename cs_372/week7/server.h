#include <arpa/inet.h>
#include <dirent.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define CMD_LS "-l"
#define CMD_GET "-g"

#define MAX_HOST_NAME 255
#define MAX_DATA 256
#define MAX_CLIENTS 10

#define PORT "9119"  // TODO: CLI...

#define ERR_GET_ADDR 1
#define ERR_BIND 2
#define ERR_LISTEN 3
#define ERR_SELECT 4

#define __DEBUG__ 1