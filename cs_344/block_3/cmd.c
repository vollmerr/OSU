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
 * Exits shell, terminates children using SIGUSR2
 */
int cmd_exit() {
  kill(0, SIGUSR2);
  return 1;
}

/**
 *  Builtin command - cd
 *  Changes directories, does not effect path outside of shell
 *  @param {char*}  token     - token already parsed once
 *  @param {char**} save_ptr  - pointer for strtok_r internals
 */
void cmd_cd(char *token, char **save_ptr) {
  char buffer[MAX_CHARS];
  // get next space delim string
  token = strtok_r(NULL, " \n", save_ptr);
  // if a string, it is path to use
  if (token) {
    strcpy(buffer, token);
    expand_pids(buffer, 1);
    if (chdir(buffer)) print_err();
  }
  // else cd to users home directory
  else {
    if (chdir(getenv(ENV_HOME))) print_err();
  }
}

/**
 *  Builtin command - status
 *  prints exit or termination status of last foreground process
 *  @param {int} child_status     - exit status of child process
 */
void cmd_status() {
  char buffer[MAX_LEN];
  // global status set to termination
  if (g_status_type) {
    write(STDOUT_FILENO, "Terminated by signal ", 21);
  }
  // global status set to normal exit
  else {
    write(STDOUT_FILENO, "Exit value ", 11);
  }
  // print term signal based off global status
  write(STDOUT_FILENO, itoa((long)g_status_signo, buffer, 10), num_len((long)g_status_signo));
  write(STDOUT_FILENO, "\n", 1);
}

/**
 *  Runs any non built in commands via execlp
 *  @param  {char*}  token        - token already parsed once
 *  @param  {char**} save_ptr     - pointer for strtok_r internals
 *  @param  {int}    bg           - determines if proccess ran in background
 */
void cmd_other(char *token, char **save_ptr, int bg) {
  pid_t child = -5;
  int child_status;
  char buffer[MAX_LEN];
  struct sigaction sa_SIGINT = {{0}}, sa_SIGTSTP = {{0}}, sa_SIGUSR2 = {{0}};
  // init sig handlers
  sigfillset(&sa_SIGINT.sa_mask);
  // create new child process
  child = fork();
  switch(child) {
  case -1:
    print_err();
    break;
  case 0:
    // child
    // init signal handlers for child processes
    sigfillset(&sa_SIGTSTP.sa_mask);
    sigfillset(&sa_SIGUSR2.sa_mask);
    sa_SIGINT.sa_handler = (!bg || g_fg_only) ? SIG_DFL : SIG_IGN;
    sa_SIGTSTP.sa_handler = SIG_IGN;
    sa_SIGUSR2.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa_SIGINT, NULL);    // (ctrl-c)
    sigaction(SIGTSTP, &sa_SIGTSTP, NULL);  // (ctrl-z)
    sigaction(SIGUSR2, &sa_SIGUSR2, NULL);   // exit
    // run command
    child_new(token, save_ptr, bg);
    break;
  default:
    // parent
    // if no & or global fg only mode set
    if (!bg || g_fg_only) {
      // init sig handlers to not ignore
      sa_SIGINT.sa_handler = handle_SIGINT;
      sigaction(SIGINT, &sa_SIGINT, NULL);  // (ctrl-c)
      // wait for child fg child processes
      waitpid(child, &child_status, 0);
      // set global status if exit successful
      if (WIFEXITED(child_status)) {
        g_status_type = STATUS_EXIT;
        g_status_signo = WEXITSTATUS(child_status);
      }
    }
    // run child in background
    else {
      write(STDOUT_FILENO, "Background pid is ", 18);
      write(STDOUT_FILENO, itoa((long)child, buffer, 10), num_len((long)child));
      write(STDOUT_FILENO, "\n", 1);
    }
    break;
  }
}
