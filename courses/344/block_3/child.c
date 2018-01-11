/////////////////////////////////////////////
//
//  FILE:     child.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Child process functons for small shell.
//
/////////////////////////////////////////////
#include "smallsh.h"

/**
 * Checks if command is for a background process
 * @param  {char*} cmd    - command to check
 * @return {in}           - 1 if background (& found), 0 otherswise
 */
int child_bg(char* cmd) {
  char* amp = &cmd[(strlen(cmd) -1)];
  // remove trailing whitespace
  while (amp > cmd && isspace((char)*amp)) amp--;
  // check if ampersand (background process) and not in fg only mode
  return (*amp == '&' && !g_fg_only);
}

/**
 * Checks all background child processes to see if they have completed
 */
void child_check() {
  pid_t child;
  int signo;
  int child_status;
  char buffer[MAX_LEN];
  // fg child killed in signal, reap and set globals for status
  if (g_status_check) {
    waitpid(-1, &child_status, 0);
    if (WIFSIGNALED(child_status)) {
      if (g_status_check) {
        g_status_type = STATUS_TERM;
        g_status_signo = WTERMSIG(child_status);
      }
    }
    g_status_check = 0;
  }
  // check background process, dont stop cmd prompt
  child = waitpid(-1, &child_status, WNOHANG);
  if (child > 0) {
    write(STDOUT_FILENO, "Background pid ", 15);
    write(STDOUT_FILENO, itoa((long)child, buffer, 10), num_len((long)child));
    write(STDOUT_FILENO, " is done: ", 10);
    // exited normally, display message with pid, set signal number
    if (WIFEXITED(child_status)) {
      signo = WEXITSTATUS(child_status);
      write(STDOUT_FILENO, "Exit value ", 11);
      write(STDOUT_FILENO, itoa((long)signo, buffer, 10), num_len((long)signo));
    }
    // exited by term signal, display message, set signal number
    else if (WIFSIGNALED(child_status)) {
      signo = WTERMSIG(child_status);
      write(STDOUT_FILENO, "Terminated by signal ", 21);
      write(STDOUT_FILENO, itoa((long)signo, buffer, 10), num_len((long)signo));
    }
    // print signal number
    write(STDOUT_FILENO, "\n", 1);
  }
}


/**
 * Sets up redirects then runs command
 * using execvp with list of arguments
 * @param {char*}   token     - main token for space seperated words
 * @param {char**}  save_ptr  - save pointer for main token
 * @param {int}     bg        - process in background if true
 */
void child_new(char *token, char **save_ptr, int bg) {
  char *args[MAX_ARGS];
  int arg, rd_in_bg, rd_in, rd_out, fd_in, fd_out;
  arg = 0;
  rd_in = 0;
  rd_in_bg = 0;
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
      if (fd_in < 0) print_err();
      // set redirect from stdin to file desc
      dup2(fd_in, STDIN_FILENO);
      close(fd_in);
      rd_in = 0;
      rd_in_bg = 1;
    }
    // next word will be out redirect file
    else if (*token == '>') {
      rd_out = 1;
    }
    // output redirect file
    else if (rd_out) {
      // get pointer to file desc
      fd_out = creat(token, 0644);
      if (fd_out < 0) print_err();
      // set redirect from stdout to file desc
      dup2(fd_out, STDOUT_FILENO);
      close(fd_out);
      rd_out = 0;
    }
    // argument to pass to exec
    else {
      strcpy(buffer, token);
      // expand any $$ to pid
      expand_pids(buffer, 0);
      // add to array of arguments
      args[arg] = malloc(sizeof(buffer));
      if (args == NULL) print_err();
      strcpy(args[arg], buffer);
      arg++;
    }
    // get next space seperated word
    token = strtok_r(NULL, " \n", save_ptr);
  }
  // redirect if in background and no input
  if (bg) {
    // redirect output to /dev/null
    fd_out = open("/dev/null", O_WRONLY);
    if (fd_out < 0) print_err();
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
    // if no input file specificied get from /dev/null
    if (!rd_in_bg) {
      fd_in = open("/dev/null", O_RDONLY);
      if (fd_in < 0) print_err();
      dup2(fd_in, STDIN_FILENO);
      close(fd_in);
    }
  }
  // ignore if background arg (already determined)
  if (*args[arg-1] == '&') {
    args[arg-1] = NULL;
  }
  // last arg must be null for execvp
  args[arg] = NULL;
  // execute the command
  execvp(args[0], args);
  print_err();
}
