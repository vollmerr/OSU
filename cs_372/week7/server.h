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
#define MAX_BUFFER 4096

#define PORT "9119"  // TODO: CLI...

#define ERR_GET_ADDR 1
#define ERR_BIND 2
#define ERR_LISTEN 3
#define ERR_SELECT 4

// #define __DEBUG__ 1

// initalization
void init_server(int *server_fd);
void init_server_info(struct addrinfo **ai);
int init_server_socket(struct addrinfo *ai);
void init_fd_sets(fd_set *master, fd_set *read_fds, int server_fd);
// core functionality
void server_loop(fd_set *master, fd_set *read_fds, int server_fd);
void handle_new_client(fd_set *master, int server_fd, int *max_fd);
void handle_recv_client(fd_set *master, int fd);
void handle_send_client(int fd, char *msg);
// commands
void handle_cmd(char *cmd, int fd);
void handle_cmd_ls(int fd);
// util
void print_debug(const char *format, ...);
char *trim(char *s);