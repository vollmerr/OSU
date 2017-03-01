/////////////////////////////////////////////
//
//  FILE:     cmd.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Command Functions for small shell.
//
/////////////////////////////////////////////
#include "smallsh.h"

/**
 * Builtin command - exit
 * Exits shell, cleans up children processes
 */
int cmd_exit() {
  return 1;
}

/**
 *  Builtin command - cd
 *  Changes directories, does not effect path outside of shell
 *  @param {char*}  token     - token already parsed once
 *  @param {char**} save_ptr  - pointer for strtok_r internals
 */
void cmd_cd(char *token, char **save_ptr) {
  // get next space delim string
  token = strtok_r(NULL, " \n", save_ptr);
  // if a string, it is path to use
  if (token) {
    chdir(token);
  }
  // else cd to users home directory
  else {
    chdir(getenv(ENV_HOME));
  }
}

/**
 *  Builtin command - status
 *  prints exit or termination status of last foreground process
 *  @param {int} child_status     - exit status of child process
 */
void cmd_status(int child_status) {
  int signo;
  char buffer[MAX_LEN];
  if (WIFSIGNALED(child_status)) {
    signo = WTERMSIG(child_status);
    write(STDOUT_FILENO, "terminated by signal ", 21);
  }
  if (WIFEXITED(child_status)) {
    signo = WEXITSTATUS(child_status);
    write(STDOUT_FILENO, "exit value ", 11);
  }
  write(STDOUT_FILENO, itoa((long)signo, buffer, 10), num_len((long)signo));
  write(STDOUT_FILENO, "\n", 1);
}

/**
 *  Runs any non built in commands via execlp
 *  @param  {char*}  token        - token already parsed once
 *  @param  {char**} save_ptr     - pointer for strtok_r internals
 *  @param  {int}    fg           - determines if proccess ran in foreground
 *  @param {int*}    child_status - status of child process return
 *  @return {int}    child_status - exit value of last fg process         -
 */
void cmd_other(char *token, char **save_ptr, int fg, int* child_status) {
  pid_t child = -5;
  char buffer[MAX_LEN];
  struct sigaction sa_SIGINT = {{0}};
  // init sig handlers
  sigfillset(&sa_SIGINT.sa_mask);
  sa_SIGINT.sa_flags = 0;
  child = fork();
  // sperate into err / child / parent
  switch(child) {
  case -1:
    perror("Failed to create new proccess.\n");
    exit(EXIT_FAILURE);
    break;
  case 0:
    // init signal handlers for fg processes
    sa_SIGINT.sa_handler = fg ? SIG_DFL : SIG_IGN;
    sigaction(SIGINT, &sa_SIGINT, NULL);  // (ctrl-c)
    // run command
    child_new(token, save_ptr);
    break;
  default:
    if (fg) {
      // init sig handlers to not ignore
      sa_SIGINT.sa_handler = handle_SIGINT;
      sigaction(SIGINT, &sa_SIGINT, NULL);  // (ctrl-c)
      // wait for child fg child processes
      waitpid(child, child_status, 0);
    }
    // run child in background
    else {
      write(STDOUT_FILENO, "background pid is ", 18);
      write(STDOUT_FILENO, itoa((long)child, buffer, 10), num_len((long)child));
      write(STDOUT_FILENO, "\n", 1);
    }
    break;
  }
}
