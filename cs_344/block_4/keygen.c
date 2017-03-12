//////////////////////////////////////////////////////////////
//  file:     keygen.c
//  author:   Ryan Vollmer
//  purpose:  Generates a key of specified length
//  usage:    keygen <key_length>
//////////////////////////////////////////////////////////////
#include "headers.h"

int generate_key() {
  int i;
  char buffer[argv[1]];
  i = 0;
  // for each character in key length
  while (i < argv[1]) {
    // set random character
    buffer[i] = VALID_CHARS[rand() % VALID_CHARS_LENGTH];
    i++;
  }
  // make into valid c string
  buffer[i] = '\0';
  fprintf(stdout, "%s", buffer);
}

int main(int argc, char *argv[]) {
  switch(argc) {
  case 2:
    generate_key();
    break;
  default:
    print_usage(argv[0], "<key_length>");
    break;
  }
  return EXIT_SUCCESS;
}
