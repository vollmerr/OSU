#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define DEBUG

#define CMD_EXIT    "exit"
#define CMD_CD      "cd"
#define CMD_STATUS  "status"

#define MAX_CHARS 2048
#define MAX_ARGS 512

/**
 *  Helper function for printing during debugging
 *  Requires DEBUG to be defined
 *  @param {char*}  func_name   - name of function to debug
 *  @param {int}    depth       - depth of print call (tabs that many times)
 *  @param {char*}  data        - data to print
 */
void print_debug(char *func_name, int depth, char *data) {
#ifdef DEBUG
  int i;
  for (i=0; i<depth; i++) {
    printf("\t");
  }
  printf(" %s: %s", func_name, data);
#endif
}

/**
 *  Builtin command - cd
 *  Changes directories, does not effect path outside of shell
 *  @param {char*}  token     - token already parsed once
 *  @param {char**} save_ptr  - pointer for strtok_r internals
 */
void cmd_cd(char *token, char **save_ptr) {
  print_debug("cmd_cd", 0);
  token = strtok_r(NULL, " \n", save_ptr);
  printf("token: %s\n", token);
  
}

/**
 *  Builtin command - status
 *  prints exit or termination status of last foreground process
 */
void cmd_status() {
  print_debug("cmd_status", 0);
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

//      remove newline char
//      buffer[strlen(buffer) - 1] = '\0';

    // get first space delim string 
    token = strtok_r(buffer, " \n", &save_ptr);
    // if not blank line and not comment 
    if (token && *token != '#') {
       printf("token: [%s]\n", token);
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
      // unknown cmd, run using exec
      else {
        printf("RUNNING EXEC\n");
      }
    }
    fflush(stdin);

//    token = strtok_r(NULL, " \n", &save_ptr);
//    printf("token: %s\n", token); 
//    token = strtok_r(NULL, " \n", &save_ptr);
//    printf("token: %s\n", token); 
//    printf("buffer: %s\n", buffer);
  } while (strcmp(buffer, CMD_EXIT));
  return 0;
}
