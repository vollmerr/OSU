/**
 * server_cmd.c
 *
 * Commands for server to perform
 */

#include "server.h"

/**
 * Handles calling the correct command
 * cmd format:     "<cmd> [arg]"
 *
 * @param {fd_set*} master      - master set of file desc
 * @param {int} fd              - file desc of client issuing command
 * @param {char**} client_port   - port to send data to
 * @param {char*} cmd           - command to call
 */
void handle_cmd(fd_set *master, int fd, char **client_ports, char *command) {
  print_debug("handle_cmd: '%d' - '%s'", fd, command);          
  char *cmd;
  char *arg;
  char delim[] = " \t\r\n\v\f";
  char *client_port = client_ports[fd - 3]; // -3 for stdout, stdin, server fd
  // get command
  cmd = strtok(command, delim);
  // get additional arg (ex filename)
  if (command != NULL) {
    arg = strtok(NULL, delim);
  }
  // match command and take action on it
  if (!strcmp(cmd, CMD_PWD)) {  // PWD
    handle_cmd_pwd(fd, client_port);
  } else if (!strcmp(cmd, CMD_RETR)) {  // RETR
    handle_cmd_retr(fd, client_port, arg);
  } else if (!strcmp(cmd, CMD_PORT)) {  // PORT
    handle_cmd_port(fd, client_ports, arg);
  } else if (!strcmp(cmd, CMD_QUIT)) {  // QUIT
    handle_cmd_quit(master, fd);
  } else {
    handle_cmd_unknown(fd);  // unknown
  }
}

/**
 * Handles listsing the current directory contents
 *
 * @param {int} fd              - file desc of client issuing command
 * @param {char*} client_port   - port to send data to
 */
void handle_cmd_pwd(int fd, const char *client_port) {
  print_debug("handle_cmd_pwd: '%d' - '%s'", fd, client_port);        
  DIR *dp;
  struct dirent *ep;
  char data[MAX_BUFFER] = "";
  char len[MAX_BUFFER] = "";
  // if data port not set, err
  if (!client_port || !strcmp(client_port, "")) {
    handle_send_code(fd, CODE_INT_ERR, "Data port not set.");
    return;
  }
  // get current dir listing
  dp = opendir("./");
  if (dp != NULL) {
    // put newline seperate list into buffer
    while ((ep = readdir(dp))) {
      strcat(data, ep->d_name);
      strcat(data, "\n");
    }
    // close dir
    (void)closedir(dp);
  } else {
    handle_send_code(fd, CODE_INT_ERR, "Failed to list directory.");
    return;
  }
  // send ok with length of data
  sprintf(len, "%zu", strlen(data));
  handle_send_code(fd, CODE_OK, len);
  // send data
  handle_send_data(fd, client_port, data, strlen(data));
}

/**
 * Handles sending a file to client
 *
 * @param {int} fd                - file desc of client issuing command
 * @param {char*} client_port     - port to send data to
 * @param {char*} file_name       - name of file to send
 */
void handle_cmd_retr(int fd, const char *client_port, const char *file_name) {  
  print_debug("handle_cmd_retr: '%d' - '%s' - '%s'", fd, client_port, file_name);      
  FILE *fp;
  struct stat *info;
  long int size;
  char *data;
  char len[MAX_BUFFER] = "";
  // if data port not set, err
  if (!client_port || !strcmp(client_port, "")) {
    handle_send_code(fd, CODE_INT_ERR, "Data port not set.");
    return;
  }
  // check if file exists, get info about it
  info = malloc(sizeof(struct stat));
  if (stat(file_name, info) != 0) {
    handle_send_code(fd, CODE_NOT_FOUND, "File not found.");
    return;
  }
  size = info->st_size;
  free(info);
  // allocate some space to store the file
  data = malloc(size);
  if (data == NULL) {
    handle_send_code(fd, CODE_INT_ERR, "Failed to allocate memory.");
    free(data);
    return;
  }
  // open the file
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    handle_send_code(fd, CODE_INT_ERR, "Failed to open file.");
    free(data);
    return;
  }
  // read the file
  if (fread(data, size, 1, fp) != 1) {
    handle_send_code(fd, CODE_INT_ERR, "Failed to read file.");
    free(data);
    fclose(fp);
    return;
  }
  // close file
  fclose(fp);
  // send ok with length of data
  sprintf(len, "%ld", size);
  handle_send_code(fd, CODE_OK, len);
  // send data
  handle_send_data(fd, client_port, data, size);
  free(data);
}

/**
 * Handles setting the clients data port
 *
 * @param {int} fd                - client fd to get data from
 * @param {char**} client_port    - list of clients data port to fill
 * @param {char*} data            - data to fill port with
 */
void handle_cmd_port(int fd, char **client_ports, const char *data) {
  print_debug("handle_cmd_port: '%d' - '%s'", fd, data);      
  char* port = malloc(SIZE_PORT * sizeof(char));
  strcpy(port, data);
  client_ports[fd - 3] = port; // -3 for stdout, stdin, server fd
  print_debug("setting client port '%s' (from '%s')\n", client_ports[fd-3], data);
  handle_send_code(fd, CODE_OK, "0");
}

/**
 * Handles closing a connection with the client
 *
 * @param {fd_set*} master   - master set of file desc
 * @param {int} fd           - client fd to get data from
 */
void handle_cmd_quit(fd_set *master, int fd) {
  print_debug("handle_cmd_quit: '%d'", fd);      
  printf("Connection closed on socket %d\n\n", fd);
  fflush(stdout);
  // clean up connection fd list
  close(fd);
  FD_CLR(fd, master);
}

/**
 * Handles unknown commands
 *
 * @param {int} fd           - client fd to get data from
 */
void handle_cmd_unknown(int fd) {
  print_debug("handle_cmd_unknown: '%d'", fd);        
  handle_send_code(fd, CODE_BAD_REQ, "Unknown command...");
}
