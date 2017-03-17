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
void valid_input(const char* msg, const char* name) {
  int i = 0;
  // check all chars are valid
  while (msg[i]) {
    if (!strchr(VALID_CHARS, msg[i])) {
      char err[MAX_LEN];
      errno = EXIT_FAILURE;
      sprintf(err, "Invalid characters in '%s'", name);
      print_err(err);
    }
    i++;
  }
}

/**
 * Checks if key is too short for msg
 * @param msg [description]
 * @param key [description]
 */
void valid_length(const char* msg, const char* key, const char* name) {
  // compare key to message length
  if (strlen(msg) > strlen(key)) {
    char err[MAX_LEN];
    errno = EXIT_FAILURE;
    sprintf(err, "Key '%s' too short", name);
    print_err(err);
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
  while (i != c && i < max) {
    i++;
  }
  return VALID_CHARS[c];
}

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
