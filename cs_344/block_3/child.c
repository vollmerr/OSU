/////////////////////////////////////////////
//
//  FILE:     child.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Child process functons for small shell.
//
/////////////////////////////////////////////
#include "smallsh.h"

/**
 * Checks all child processes to see if they have completed
 */
void child_check() {
  pid_t child;
  int child_status;
  char buffer[MAX_LEN];
  child = waitpid(-1, &child_status, WNOHANG);
  if (child > 0) {
    if (WIFEXITED(child_status)) {
      write(STDOUT_FILENO, "background pid ", 15);
      write(STDOUT_FILENO, itoa((long)child, buffer, 10), num_len((long)child));
      write(STDOUT_FILENO, " is done: ", 10);
    }
    cmd_status(child_status);
  }
}


/**
 * Sets up redirects then runs the command
 * using execvp with list of arguments
 * @param {char*}   token     - main token for space seperated words
 * @param {char**}  save_ptr  - save pointer for main token
 */
void child_new(char *token, char **save_ptr) {
  char *args[MAX_ARGS];
  int arg, rd_in, rd_out, fd_in, fd_out;
  arg = 0;
  rd_in = 0;
  rd_out = 0;
  // go through each space seperated word
  while (token) {
    char buffer[MAX_CHARS];
    // next word will be input redirect file
    if(*token == '<') {
      rd_in = 1;
    }
    // input redirect file
    else if (rd_in) {
      // get pointer to file desc
      fd_in = open(token, O_RDONLY);
      if (fd_in < 0) {

        exit(EXIT_FAILURE);
      };
      // set redirect from stdin to file desc
      dup2(fd_in, STDIN_FILENO);
      close(fd_in);
      rd_in = 0;
    }
    // next word will be out redirect file
    else if (*token == '>') {
      rd_out = 1;
    }
    // output redirect file
    else if (rd_out) {
      // get pointer to file desc
      fd_out = creat(token, 0644);
      if (fd_out < 0) exit(EXIT_FAILURE);
      // set redirect from stdout to file desc
      dup2(fd_out, STDOUT_FILENO);
      close(fd_out);
      rd_out = 0;
    }
    // argument to pass to exec
    else {
      strcpy(buffer, token);
      // expand any $$ to pid
      expand_pids(buffer);
      // add to array of arguments
      args[arg] = malloc(sizeof(buffer));
      if (args == NULL) exit(EXIT_FAILURE);
      // ignore if background arg (already determined)
      if (*token == '&') {
        args[arg] = NULL;
      }
      else {
        strcpy(args[arg], buffer);
      }
      arg++;
    }
    // get next space seperated word
    token = strtok_r(NULL, " \n", save_ptr);
  }
  // last arg must be null for execvp
  args[arg] = NULL;
  // execute the command
  execvp(args[0], args);
  perror(NULL);
  exit(EXIT_FAILURE);
}
