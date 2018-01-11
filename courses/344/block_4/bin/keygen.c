//////////////////////////////////////////////////////////////
//  file:     keygen.c
//  author:   Ryan Vollmer
//  purpose:  Generates a key of specified length
//  usage:    keygen <key_length>
//////////////////////////////////////////////////////////////
#include "headers.h"

void generate_key(const int len) {
  int i;
  char buffer[len + 1];
  i = 0;
  // for each character in key length
  while (i < len) {
    // set random character
    buffer[i] = VALID_CHARS[rand() % VALID_CHARS_LENGTH];
    i++;
  }
  // add newline
  buffer[i] = '\n';
  // make into valid c string
  buffer[i+1] = '\0';
  fprintf(stdout, "%s", buffer);
}

int main(int argc, char *argv[]) {
  if (argc < 2) print_usage(argv[0], "<key_length>");
  srand((unsigned)time(NULL));
  generate_key(atoi(argv[1]));
  return EXIT_SUCCESS;
}
