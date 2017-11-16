/**
 * server_util.c
 * 
 * Common utility helper function
 */

#include "server.h"

/**
 * Helper function for printing debug messages
 *
 * @param {char*} format    - format for message to display
 * @param {vargs} ...       - args for format to render
 */
void print_debug(const char *format, ...) {
#ifdef __DEBUG__
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  printf("\n");
  va_end(args);
#endif
}

/**
 * Removes newline characters
 */
char *trim(char *s) {
  int i = strlen(s) - 1;
  if ((i > 0) && (s[i] == '\n')) {
    s[i] = '\0';
  }
  return s;
}