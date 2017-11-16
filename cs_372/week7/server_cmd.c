/**
 * server_cmd.c
 * 
 * Commands for server to perform
 */

#include "server.h"

/**
 * Handles listsing the current directory contents
 */
void handle_cmd_ls() {
  print_debug("handle_cmd_ls");

  DIR *dp;
  struct dirent *ep;
  char buffer[MAX_BUFFER];

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

  // handle_send_client(buffer);

  print_debug("handle_cmd_ls, buffer: %s", buffer);
  print_debug("/handle_cmd_ls");
}

/**
 * Handles calling the correct command
 *
 * @param {char*} cmd       - command to call
 */
void handle_cmd(char *cmd) {
  print_debug("handle_cmd");

  if (strstr(cmd, CMD_LS)) {
    handle_cmd_ls();
  } else {
    printf("DEFAULT");
  }

  print_debug("/handle_cmd");
}