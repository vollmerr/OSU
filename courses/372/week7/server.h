#include <arpa/inet.h>
#include <dirent.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CMD_PWD "PWD"    // get current working dir list
#define CMD_RETR "RETR"  // Retrieve a copy of the file
#define CMD_PORT "PORT"  // send data port number
#define CMD_QUIT "QUIT"  // quit...

#define SIZE_CODE 4  // 3 + '\0'
#define SIZE_PORT 6
#define MAX_HOST_NAME 255
#define MAX_DATA 256
#define MAX_CLIENTS 10
#define MAX_BUFFER 4096

#define ERR_GET_ADDR 1
#define ERR_BIND 2
#define ERR_LISTEN 3
#define ERR_SELECT 4
#define ERR_FILE_OPEN 5

#define CODE_OK "200"
#define CODE_BAD_REQ "400"
#define CODE_NOT_FOUND "404"
#define CODE_INT_ERR "500"

// #define __DEBUG__ 1

// initalization
void init_server(int *server_fd, const char *port);
void init_server_info(struct addrinfo **ai, const char *port);
int init_server_socket(struct addrinfo *ai);
void init_fd_sets(fd_set *master, fd_set *read_fds, int server_fd);
// core functionality
void handle_new_client(fd_set *master, int server_fd, int *max_fd);
void handle_recv_client(fd_set *master, int fd, char **client_ports);
void handle_send_client(int fd, const char *msg, long int len);
void handle_send_code(int fd, const char *code, const char *desc);
void handle_send_data(int fd, const char *port, const char *msg, long int len);
void server_loop(fd_set *master, fd_set *read_fds, int server_fd);
// commands
void handle_cmd(fd_set *master, int fd, char **client_ports, char *command);
void handle_cmd_pwd(int fd, const char *client_port);
void handle_cmd_retr(int fd, const char *client_port, const char *file_name);
void handle_cmd_port(int fd, char **client_ports, const char *data);    
void handle_cmd_quit(fd_set *master, int fd);
void handle_cmd_unknown(int fd);
// util
void print_debug(const char *format, ...);
void print_clear();
char *trim(char *s);