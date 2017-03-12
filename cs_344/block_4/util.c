#include "headers.h"

void print_error(const char* err, int code) {
  perror(err);
  exit(code);
}

void print_usage(const char* prog, const char* msg) {
  fprintf(stderr,"USAGE: %s %s\n", prog, msg);
  exit(EXIT_FAILURE);
}
