/////////////////////////////////////////////
//
//  FILE:     smallsh.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Main entrance to small shell program.
//  USAGE:    command [arg1 arg2 ...] [< input_file] [> output_file] [&]
//
/////////////////////////////////////////////
#include "smallsh.h"

int main() {
  char buffer[MAX_CHARS];
  char *save_ptr, *token;
  int fg, done = 0;
  // // init global flags
  // g_status_type = 0;
  // g_status_signo = 0;
  // g_fg_only = 0;
  g_status_check = 0;
  // init sig handlers
  struct sigaction sa_SIGINT = {{0}}, sa_SIGTSTP = {{0}};
  sigfillset(&sa_SIGINT.sa_mask);
  sigfillset(&sa_SIGTSTP.sa_mask);
  sa_SIGTSTP.sa_handler = handle_SIGTSTP;
  sigaction(SIGTSTP, &sa_SIGTSTP, NULL);  // (ctrl-z)
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
    // check if background proccess
    fg = strstr(buffer, "&") ? 0 : 1;
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
        cmd_other(token, &save_ptr, fg);
      }
    }
    fflush(stdout);
  } while (!done);
  return 0;
}
