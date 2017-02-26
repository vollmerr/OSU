#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define CMD_EXIT    "exit"
#define CMD_CD      "cd"
#define CMD_STATUS  "status"

#define ENV_HOME    "HOME"
#define ENV_PATH    "PATH"
#define ENV_PWD     "PWD"

#define MAX_CHARS   2048
#define MAX_ARGS    512
#define MAX_LEN     64
#define MAX_CHILD   5
/*
void catch_SIGCHLD(int signo) {
 char* message = "Caught SIGINT, sleeping for 5 seconds\n";
  write(STDOUT_FILENO, message, 38);
}
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_handler = catch_SIGCHLD;
  sa.sa_flags = 0;
  sigaction(SIGCHLD, &sa, NULL);
*/
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
 */
void cmd_status() {
  return;
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
    // ignore if background arg (already determined)
    if (*token == '&') {
      continue;
    }
    // next word will be input redirect file
    if(*token == '<') {
      rd_in = 1;
    }
    // input redirect file
    else if (rd_in) {
      // get pointer to file desc
      int fd_in = open(token, O_RDONLY);
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
      strcpy(args[arg], buffer);
      arg++;
    }
    // get next space seperated word
    token = strtok_r(NULL, " \n", save_ptr);
  }
  // last arg must be null for execvp
  args[arg] = NULL;
  // execute the command
  execvp(args[0], args);
}

/**
 * Checks all children to see if they have completed
 */
void child_check(pid_t *children, int *child_num) {
printf("CHECKING CHILDREN\n");
  pid_t child;
  int child_status;
  int i;
printf("child nu : %d\n",*child_num);
//       child =  waitpid(-1, &child_status, 0);
  child = waitpid(-1, &child_status, WNOHANG);
printf("WTFFF CHILD:::::: %ld\n", (long)child);
  for (i=0; i<(*child_num); i++) {
printf("checking child: %ld\n", (long)children[i]);
printf("CHILD IS: %ld\n", child);
    if (child > 0) {
      printf("background pid %ld is done: TODO_STATUS\n", (long)child); 
    }
printf("WTF\n");
  }
}

/**
 *  Runs any non built in commands via execlp
 *  @param {char*}  token     - token already parsed once
 *  @param {char**} save_ptr  - pointer for strtok_r internals
 */
void cmd_other(char *token, char **save_ptr, int fg, pid_t *children, int *child_num) {
  pid_t child = -5;
  int child_status = -5;
  child = fork();
  switch(child) {
    case -1:
      perror("Failed to create new proccess.\n");
      exit(1);
      break;
    case 0: 
      // run command
      child_new(token, save_ptr);
      break;
    default:
      // run child in foreground
      if (fg) {
        waitpid(child, &child_status, 0);
      }
      // run child in background
      else {
        printf("background pid is %ld\n", (long)child);
        children[*child_num] = child;
printf("child_num: %d", *child_num);
        printf("CHILD CREATED:: \n", children[(*child_num)]);
        *child_num = *child_num + 1;
      }
      break;
  }
}

int main() {
  char buffer[MAX_CHARS];
  char *save_ptr, *token;
  int fg, child_num;
  pid_t children[MAX_CHILD]; 
pid_t pid;
int status;
/*  
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  sigaction(SIGCHLD, &sa, NULL);
*/

  child_num = 0;
  // clear screen
  //printf("\e[1;1H\e[2J");
  // do while exit not entered
  do {
    // check for completed processes
//    child_check(children, &child_num);

pid = waitpid(-1, &status, WNOHANG);
                if (pid > 0)
                        printf("waitpid reaped child pid %d\n", pid);

    // print prompt
    printf(":");
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
        cmd_status();
      }
      // other non-builitn cmd, run using exec
      else {
        cmd_other(token, &save_ptr, fg, children, &child_num);
      }
    }
    fflush(stdout);
  } while (strcmp(buffer, CMD_EXIT));
  return 0;
}
