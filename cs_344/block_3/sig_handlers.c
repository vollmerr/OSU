/////////////////////////////////////////////
//
//  FILE:     util.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Signal Handlers.
//
/////////////////////////////////////////////

#include "smallsh.h"
/**
 * Handles SIGINTS (ctrl-c)
 * @param {int} signo   - signal number recieved
 */
void handle_SIGINT (int signo) {
  pid_t child;
  int child_status;
  char buffer[MAX_LEN];
  char* message = "terminated by signal ";
  // check if SIGINT was sent while child process was running
  child = waitpid(-1, &child_status, 0);
  if (WIFSIGNALED(child_status)) {
    write(STDOUT_FILENO, message, 21);
    write(STDOUT_FILENO, itoa(signo, buffer, 10), 1);
  }
  write(STDOUT_FILENO, "\n", 1);
}
