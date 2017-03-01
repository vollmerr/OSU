/////////////////////////////////////////////
//
//  FILE:     util.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Signal Handlers.
//
/////////////////////////////////////////////
#include "smallsh.h"

/**
 * Handles SIGINTs (ctrl-c)
 * @param {int} signo   - signal number recieved
 */
void handle_SIGINT (int signo) {
  char buffer[MAX_LEN];
  char* message = "terminated by signal ";
  // set global status so reaped next cycle
  g_status_check = 1;
  // message term signal
  write(STDOUT_FILENO, message, 21);
  write(STDOUT_FILENO, itoa(signo, buffer, 10), 1);
  write(STDOUT_FILENO, "\n", 1);
}

/**
 * Handles SIGTSTPs (ctrl-z)
 * @param {int} signo   - signal number recieved
 */
void handle_SIGTSTP (int signo) {
  // toggle forgedoun only mmode
  g_fg_only = !g_fg_only;
  // display message of fg only mode or not
  if (g_fg_only) {
    write(STDOUT_FILENO, "Entering foreground-only mode (& is now ignored)\n", 49);
  }
  else {
    write(STDOUT_FILENO, "Exiting foreground-only mode\n", 29);
  }
}
