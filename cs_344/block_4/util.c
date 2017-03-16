#include "headers.h"

void print_err(const char *err) {
  fprintf( stderr, "Error: %s\n", err);
  exit(errno);
}

void print_usage(const char* prog, const char* msg) {
  printf("USAGE: %s %s\n", prog, msg);
  exit(EXIT_FAILURE);
}

/**
 * Checks if input is valid
 * @param  msg [description]
 * @return     [description]
 */
void valid_input(const char* msg) {
  int i = 0;
  // check all chars are valid
  while (msg[i]) {
    if (!strchr(VALID_CHARS, msg[i])) {
      errno = EXIT_FAILURE;
      print_err("Invalid characters");
    }
    i++;
  }
}

/**
 * Converts a character to int representation based off valid chars
 * @param  c [description]
 * @return   [description]
 */
int char_to_i(const char c) {
  int i=0;
  int max = 27;
  while (c != VALID_CHARS[i] && i < max) {
    i++;
  }
  return i;
}

/**
 * Converts an int to char based off valid chars
 * @param  c [description]
 * @return   [description]
 */
char char_from_i(const int c) {
  int i=0;
  int max = 27;
  while (i != VALID_CHARS[c] && i < max) {
    i++;
  }
  return VALID_CHARS[c];
}
