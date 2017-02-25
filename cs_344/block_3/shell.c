#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    getcwd(cwd, sizeof(cwd));
    printf("cwd: %s\n", cwd);
    printf("token: %s\n", token);
  }
  // else cd to users home directory
  else {
    chdir(getenv(ENV_HOME));
    printf("HOME: %s\n", getenv(ENV_HOME));
    getcwd(cwd, sizeof(cwd));
    printf("cwd: %s\n", cwd);
  }
}

/**
 *  Builtin command - status
 *  prints exit or termination status of last foreground process
 */
void cmd_status() {
  return;
}

void expand_pid(char *str, const char *var, const char *pid) {
  char buffer[MAX_CHARS];
  char *insert = buffer;
  char *ptr;
  char *tmp = str;
  size_t var_len = strlen(var);
  size_t pid_len = strlen(pid);
  while(1) {
    ptr = strstr(tmp, var);
    if (!ptr) {
      strcpy(insert, tmp);
      break;
    }
    memcpy(insert, tmp, ptr - tmp);
    insert += ptr - tmp;
    memcpy(insert, pid, pid_len);
    insert += pid_len;
    tmp = ptr + var_len;
  }
  strcpy(str, buffer);
}

void run_child(char *token, char **save_ptr, pid_t pid) {
  char *args[MAX_ARGS];
  char *var;
//  char var_start[MAX_LEN], var_end[MAX_LEN], var_whole[MAX_LEN];
  char pid_buffer[MAX_LEN];
  char buffer[MAX_CHARS];
  int arg, rd_in, rd_out, fd_in, fd_out;
  arg = 0;
  rd_in = 0;
  rd_out = 0;
  while (token) {
    if(*token == '<') {
      rd_in = 1;
    }
    else if (rd_in) {
      int fd_in = open(token, O_RDONLY);
      dup2(fd_in, STDIN_FILENO);
      close(fd_in);
      rd_in = 0;
    }
    else if (*token == '>') {
      rd_out = 1;
    }
    else if (rd_out) {
      int fd_out = creat(token, 0644);
      dup2(fd_out, STDOUT_FILENO);
      close(fd_out);
      rd_out = 0;
    }
    else {
      strcpy(buffer, token);
      expand_pid(buffer, "$$", "12345");
 //     var = strstr(token, "$$");
 //     if (var) {
 //       strcpy(var_start, token);
 //       strcpy(var_end, var - token);
 //       printf("VAR_START: %s", var_start);
 //       printf("VAR_END: %s", var_end);
 //
/*
        strcpy(var_buffer, "");
 printf("$$ found: %s\n", token);
        var = strtok_r(token, "$$", &var_save);
 printf("$$ var: %s\n", var);
        //sprintf(var_buffer, "%s", var);
        sprintf(pid_buffer, "%d", pid);
        while (var) {
          strcat(var_buffer, var);
          strcat(var_buffer, pid_buffer);
          var = strtok_r(NULL, "$$", &var_save);
        } 
 printf("$$ buffer: %s\n", var_buffer);
*/ 
    // }
      printf("token is:::: %s\n", buffer);
      args[arg++] = buffer;
    }
    token = strtok_r(NULL, " \n", save_ptr);
  }
  // last arg must be null for execvp
  args[++arg] = NULL;
  // execute the command
  execvp(args[0], args);
}

/**
 *  Runs any non built in commands via execlp
 *  @param {char*}  token     - token already parsed once
 *  @param {char**} save_ptr  - pointer for strtok_r internals
 */
void cmd_other(char *token, char **save_ptr) {
  pid_t child = -5;
  int child_status = -5;
  child = fork();
  switch(child) {
    case -1:
      perror("Failed to create new proccess.\n");
      exit(1);
      break;
    case 0: 
      printf("IN CHILD: %d\n", child);
      // run command
      run_child(token, save_ptr, child);
      break;
    default:
      
    token = strtok_r(NULL, " \n", save_ptr);
  printf("PARENT: token: %s", token);
      printf("PARENT: watiing on %d.\n", child);
      waitpid(child, &child_status, 0);
      printf("PARENT: child term.\n");
      break;
  }
}

int main() {
  char *buffer;
  char *save_ptr, *token;
  // clear screen
  //printf("\e[1;1H\e[2J");
  // do while exit not entered
  do {
    // print prompt
    printf(":");
    // get user input
    fgets(buffer, MAX_CHARS, stdin);
    // get first space delim string 
    token = strtok_r(buffer, " \n", &save_ptr);
    // if not blank line and not comment 
    if (token && *token != '#') {
      // builtin cmd to exit
      if (!strcmp(token, CMD_EXIT)) {
        printf("EXITING\n");
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
        cmd_other(token, &save_ptr);
      }
    }
    fflush(stdin);
  } while (strcmp(buffer, CMD_EXIT));
  return 0;
}
