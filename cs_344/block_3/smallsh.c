#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define CMD_EXIT    "exit"
#define CMD_CD      "cd"
#define CMD_STATUS  "status"

#define ENV_HOME    "HOME"
#define ENV_PATH    "PATH"
#define ENV_PWD     "PWD"

#define MAX_CHARS   2048
#define MAX_ARGS    512
#define MAX_LEN     64

/**
 * Converts integer to string
 * (Apparently itoa isnt standard...)
 * @param  {int}    value   - number to convert
 * @param  {char*}  result  - buffer to fill
 * @param  {int}    base    - base of number
 * @return result
 */
char* itoa (int value, char *result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }
  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;
  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );
  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

/**
 * Find number of integers in number
 * @param {int}  i  - integer to get size of
 * @return - number of integers
 */
int num_len(unsigned i) {
    int n = 1;
    while (i > 9) {
        n++;
        i /= 10;
    }
    return n;
}

/**
 *  Builtin command - cd
 *  Changes directories, does not effect path outside of shell
 *  @param {char*}  token     - token already parsed once
 *  @param {char**} save_ptr  - pointer for strtok_r internals
 */
void cmd_cd(char *token, char **save_ptr) {
  char cwd[MAX_CHARS];
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
 * Expands any occurances of $$ to the porcesses pid
 * @param {char*} str     - string to expand variables in
 */
void expand_pids(char *str) {
  char buffer[MAX_CHARS];
  char pid[MAX_LEN];
  char var[] = "$$";
  char *insert = buffer;
  char *ptr;
  char *tmp = str;
  size_t var_len = strlen(var);
  size_t pid_len;
  // set pid as string
  sprintf(pid, "%ld", (long)getpid());
  pid_len = strlen(pid);
  while(1) {
    // find next occurance of variable in string
    ptr = strstr(tmp, var);
    // if no occurance found
    if (!ptr) {
      // add on last of string and exit
      strcpy(insert, tmp);
      break;
    }
    // copy string before var
    memcpy(insert, tmp, ptr - tmp);
    insert += ptr - tmp;
    // copy pid
    memcpy(insert, pid, pid_len);
    insert += pid_len;
    // move pointers to next location
    tmp = ptr + var_len;
  }
  // set passed in string as string with expanded variables
  strcpy(str, buffer);
}

/**
 * Sets up redirects then runs the command
 * using execvp with list of arguments
 * @param {char*}   token     - main token for space seperated words
 * @param {char**}  save_ptr  - save pointer for main token
 */
void child_new(char *token, char **save_ptr) {
  char *args[MAX_ARGS];
  char *var;
  char pid_buffer[MAX_LEN];
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
      int fd_in = open(token, O_RDONLY);
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
      int fd_out = creat(token, 0644);
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
      if (!args) exit(EXIT_FAILURE);
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

/**
 * Checks all children to see if they have completed
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
 * Handles SIGINTS (ctrl-c)
 * @param {int} signo   - signal number recieved
 */
void handle_SIGINT (int signo) {
  pid_t child;
  int child_status, buffer_len;
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
  struct sigaction sa_SIGINT = {0};
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

int main() {
  char buffer[MAX_CHARS];
  char *save_ptr, *token;
  int fg, fg_mode;
  pid_t pid;
  int child_status = 0;
  struct sigaction sa_SIGINT = {0};
  sigfillset(&sa_SIGINT.sa_mask);

  // clear screen
  //printf("\e[1;1H\e[2J");
  // do while exit not entered
  do {
    // reset sig handlers to ignore
    sa_SIGINT.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa_SIGINT, NULL);  // (ctrl-c)
    // printf("CHILD STATUS: %d", child_status);
    // clear buffer
    memset(buffer, '\0', sizeof(buffer));
    // check for completed processes
    child_check();
    // print prompt
    write(STDOUT_FILENO, ":", 1);
    // get user input
    fgets(buffer, MAX_CHARS, stdin);
    // check if background proccess
    fg = strstr(buffer, "&") ? 0 : 1;
    // get first space delim string
    token = strtok_r(buffer, " \n", &save_ptr);
    // if not blank line and not comment
    if (token && *token != '#') {
      // builtin cmd to exit
      if (!strcmp(token, CMD_EXIT)) {
        break;
      }
      // builtin cmd to change directory
      else if (!strcmp(token, CMD_CD)) {
        cmd_cd(token, &save_ptr);
      }
      // builtin cmd to view status of last cmd
      else if (!strcmp(token, CMD_STATUS)) {
        cmd_status(child_status);
      }
      // other non-builitn cmd, run using exec
      else {
        cmd_other(token, &save_ptr, fg, &child_status);
      }
      fflush(stdout);
    }
  } while (1);
  return 0;
}
