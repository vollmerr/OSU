/**
 * server_cmd.c
 *
 * Commands for server to perform
 */

#include "server.h"

/**
 * Handles listsing the current directory contents
 *
 * @param {int} fd      - file desc of client issuing command
 * @param {char*} port      - port to send data to
 */
void handle_cmd_pwd(int fd, char *port) {
  DIR *dp;
  struct dirent *ep;
  char data[MAX_BUFFER] = "";
  char len[MAX_BUFFER] = "";
  // if data port not set, err
  if (!strcmp(port,"")) {
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
  handle_send_data(fd, port, data);
}

/**
 * Handles sending a file to client
 *
 * @param {int} fd            - file desc of client issuing command
 * @param {char*} port        - port to send data to
 * @param {char*} file_name   - name of file to send
 */
void handle_cmd_retr(int fd, char *port, char *file_name) {
  FILE *fp;
  struct stat info;
  char *data; 
  char len[MAX_BUFFER] = "";
  // check if file exists, get info about it
  if (stat(file_name, &info) != 0) {
    handle_send_code(fd, CODE_NOT_FOUND, "File not found.");
    return;
  }
  // allocate some space to store the file
  data = malloc(info.st_size);
  if (data == NULL) {
    handle_send_code(fd, CODE_INT_ERR, "Failed to allocate memory.");
    return;
  }
  // open the file
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    handle_send_code(fd, CODE_INT_ERR, "Failed to open file.");
    return;
  }
  // read the file
  if (fread(data, info.st_size, 1, fp) != 1) {
    handle_send_code(fd, CODE_INT_ERR, "Failed to open file.");
    fclose(fp);
    return;
  }
  // close file
  fclose(fp);
  // send ok with length of data
  sprintf(len, "%zu", strlen(data));
  handle_send_code(fd, CODE_OK, len);
  // send data
  handle_send_data(fd, port, data);
}

/**
 * Handles setting the clients data port
 *
 * @param {int} fd                - client fd to get data from
 * @param {cahr*} port            - clients port to fill
 * @param {char*} data            - data to fill port with
 */
void handle_cmd_port(int fd, char *port, char *data) {
  strcpy(port, data);
  handle_send_code(fd, CODE_OK, "0");
}

/**
 * Handles closing a connection with the client
 *
 * @param {fd_set*} master   - master set of file desc
 * @param {int} fd           - client fd to get data from
 */
void handle_cmd_quit(fd_set *master, int fd) {
  printf("Connection closed on socket %d", fd);
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
  handle_send_code(fd, CODE_BAD_REQ, "Unknown command...");
}

/**
 * Handles calling the correct command
 * cmd format:     "<cmd> [arg]"
 *
 * @param {fd_set*} master  - master set of file desc
 * @param {int} fd          - file desc of client issuing command
 * @param {char*} port      - port to send data to
 * @param {char*} cmd       - command to call
 */
void handle_cmd(fd_set *master, int fd, char *port, char *command) {
  char *cmd;
  char *arg;
  char delim[] = " \t\r\n\v\f";
  // get command
  cmd = strtok(command, delim);
  // get additional arg (ex filename)
  if (command != NULL) {
    arg = strtok(NULL, delim);
  }
  printf("handle_cmd: port: '%s', command: '%s', cmd: %s, arg: %s\n ", port,
         command, cmd, arg);
  // match command and take action on it
  if (!strcmp(cmd, CMD_PWD)) {  // PWD
    handle_cmd_pwd(fd, port);
  } else if (!strcmp(cmd, CMD_RETR)) {  // RETR
    handle_cmd_retr(fd, port, arg);
  } else if (!strcmp(cmd, CMD_PORT)) {  // PORT
    handle_cmd_port(fd, port, arg);
  } else if (!strcmp(cmd, CMD_QUIT)) {  // QUIT
    handle_cmd_quit(master, fd);
  } else {
    handle_cmd_unknown(fd);  // unknown
  }
}