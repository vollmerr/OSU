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
 */
void handle_cmd_ls(int fd) {
  print_debug("handle_cmd_ls");

  DIR *dp;
  struct dirent *ep;
  char buffer[MAX_BUFFER] = "";

  dp = opendir("./");
  // opened dir
  if (dp != NULL) {
    // put newline seperate list into buffer
    while ((ep = readdir(dp))) {
      strcat(buffer, ep->d_name);
      strcat(buffer, "\n");
    }
    // close dir
    (void)closedir(dp);
  } else {
    char *err = "Failed to list directory.";
    strcpy(buffer, err);
    perror(err);
  }

  handle_send_client(fd, buffer);

  print_debug("handle_cmd_ls, buffer: %s", buffer);
  print_debug("/handle_cmd_ls");
}

/**
 * Handles sending a file to client
 *
 * @param {int} fd            - file desc of client issuing command
 * @param {char*} file_name   - name of file to send
 */
void handle_cmd_get(int fd, char *file_name) {
  print_debug("handle_cmd_get");
  print_debug("handle_cmd_get, file_name: '%s'", file_name);

  FILE *fp;
  struct stat info;
  char *buffer;
  // check if file exists, get info about it
  if (stat(file_name, &info) != 0) {
    // send not found error to client
    handle_send_client(fd, "FILE NOT FOUND");
    return;
  }
  // allocate some space to store the file
  buffer = malloc(info.st_size);
  if (buffer == NULL) {
    perror("Failed to allocate buffer");
    return;
  }
  // open the file
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    perror("Failed to open file");
    return;
  }
  // read the file
  if (fread(buffer, info.st_size, 1, fp) != 1) {
    perror("Failed to read file");
    fclose(fp);
    return;
  }
  // close file
  fclose(fp);
  // send file to client
  handle_send_client(fd, buffer);

  print_debug("/handle_cmd_get");
}

/**
 * Handles calling the correct command
 *
 * cmd format     <cmd> <port> [arg]
 * 
 * @param {char*} cmd       - command to call
 * @param {int} fd          - file desc of client issuing command
 */
void handle_cmd(char *command, int fd) {
  print_debug("handle_cmd");

  char* cmd;
  char* port;
  char* arg;
  char delim[] = " \t\r\n\v\f";
  // get command
  cmd = strtok(command, delim);
  printf("cmd: '%s'", cmd);
  // get port
  if (command != NULL) {
    port = strtok(NULL, delim);
    printf("port: '%s'", port);
  }
  // get additional arg (ex filename)
  if (command != NULL) {
    arg = strtok(NULL, delim);
    printf("arg: '%s'", arg);
  }
  // printf("cmd: %s, port: %s, arg: %s", cmd, port, arg);
  // send(fd, "testing...", 1, 0);
  // match command and take action on it
  if (strstr(cmd, CMD_LS)) {
    handle_cmd_ls(fd);
  } else if (strstr(cmd, CMD_GET)) {
    handle_cmd_get(fd, arg);
  } else {
    printf("DEFAULT");
  }

  print_debug("/handle_cmd");
}