/////////////////////////////////////////////
//
//  FILE:     util.c
//  AUTHOR:   Ryan Vollmer
//  PURPOSE:  Utility Functions for small shell.
//
/////////////////////////////////////////////
#include "smallsh.h"

/**
 * Prints error message based off last error and exits with error code
 */
void print_err() {
  perror(NULL);
  exit(errno);
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
 * Expands any occurances of $$ to the parents process pid
 * @param {char*} str     - string to expand variables in
 * @param {int}   parent  - 1 if process is the parent
 */
void expand_pids(char *str, int parent) {
  char buffer[MAX_CHARS];
  char pid[MAX_LEN];
  char var[] = "$$";
  char *insert = buffer;
  char *ptr;
  char *tmp = str;
  size_t var_len = strlen(var);
  size_t pid_len;
  // set pid as string
  sprintf(pid, "%ld", parent ? (long)getpid() : (long)getppid());
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
