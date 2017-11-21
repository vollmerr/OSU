#include "server.h"

int main(int argc, char *argv[]) {
  fd_set master;
  fd_set read_fds;
  int server_fd;

  if (argc != 2) {
    printf("Usage: %s PORT", argv[0]);
    exit(1);
  }

  init_server(&server_fd, argv[1]);
  init_fd_sets(&master, &read_fds, server_fd);
  server_loop(&master, &read_fds, server_fd);

  return EXIT_SUCCESS;
}
