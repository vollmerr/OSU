/////////////////////////////////////////////
//
//  FILE:     smallsh.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Main entrance to small shell program.
//  USAGE:    command [arg1 arg2 ...] [< input_file] [> output_file] [&]
//
/////////////////////////////////////////////
#include "smallsh.h"

/**
 * Initalize smallsh's signal handlers and globals
 * @param {sigaction*} sa_SIGINT     - handles SIGINT
 * @param {sigaction*} sa_SIGTSTP    - handles SIGTSTP
 */
void init_smallsh (struct sigaction* sa_SIGINT) {
  struct sigaction  sa_SIGTSTP = {{0}}, sa_SIGUSR2 = {{0}};
  // init global flags
  g_status_type = 0;
  g_status_signo = 0;
  g_fg_only = 0;
  g_status_check = 0;
  // init signal handlers
  sigfillset(&sa_SIGINT->sa_mask);
  sigfillset(&sa_SIGTSTP.sa_mask);
  sigfillset(&sa_SIGUSR2.sa_mask);
  sa_SIGTSTP.sa_handler = handle_SIGTSTP;
  sa_SIGUSR2.sa_handler = SIG_IGN;
  sigaction(SIGTSTP, &sa_SIGTSTP, NULL);   // (ctrl-z)
  sigaction(SIGUSR2, &sa_SIGUSR2, NULL);   // exit
}

int main() {
  char buffer[MAX_CHARS];
  char *save_ptr, *token;
  int bg, done = 0;
  struct sigaction sa_SIGINT = {{0}};
  // init handlers and globals
  init_smallsh(&sa_SIGINT);
  // do while exit not entered
  do {
    // reset sig handlers to ignore
    sa_SIGINT.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa_SIGINT, NULL);  // (ctrl-c)
    // clear buffer
    memset(buffer, '\0', sizeof(buffer));
    // check for completed processes
    child_check();
    // print prompt
    write(STDOUT_FILENO, ":", 1);
    // get user input
    fflush(stdin);
    fgets(buffer, MAX_CHARS, stdin);
    // check if foreground proccess
    bg = child_bg(buffer);
    // get first space delim string
    token = strtok_r(buffer, " \n", &save_ptr);
    // if not blank line and not comment
    if (token && *token != '#') {
      // builtin cmd to exit
      if (!strcmp(token, CMD_EXIT)) {
        done = cmd_exit();
      }
      // builtin cmd to change directory
      else if (!strcmp(token, CMD_CD)) {
        cmd_cd(token, &save_ptr);
      }
      // builtin cmd to view status of last cmd
      else if (!strcmp(token, CMD_STATUS)) {
        cmd_status();
      }
      // other non-builitn cmd, run using exec
      else {
        cmd_other(token, &save_ptr, bg, &sa_SIGINT);
      }
    }
    fflush(stdout);
  } while (!done);
  return 0;
}
