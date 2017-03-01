#ifndef __SM_SH
#define __SM_SH

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

// signal handlers
void handle_SIGINT (int signo);

// utility functions
char* itoa        (int value, char *result, int base);
int   num_len     (unsigned i);
void  expand_pids (char *str);

// commands
int   cmd_exit     ();
void  cmd_cd       (char *token, char **save_ptr);
void  cmd_status   (int child_status);
void  cmd_other    (char *token, char **save_ptr, int fg, int* child_status);

// child process functons
void  child_check  ();
void  child_new    (char *token, char **save_ptr);

#endif /* / __SM_SH */
