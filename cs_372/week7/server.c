// written in C
// NO sendfile or predfiened functions that make trivial
// short and long files
// .txt file (extra for binary)
// CITE SOURCES AS YOU GO!
// CHECK OUTPUT / INPUT FROM PDF

// validate cli params
// wait on PORTNUM for clients
// establish TCP control conneciton with client
// wait for commands
// recieve invalid command = send error
// init TCP data connection on DATA_PORT
// recieve -l = send dir to client
// recieve -g FILENAME
// validate
// send contents of file
// send error if error (not found...)
// close conneciton with client
// loop - wait on PORTNUM for clients (#2)

// bonus = cd command, username/password to access server, binary files

#include "server.h"

int main() {
  fd_set master;
  fd_set read_fds;
  int server_fd;

  init_server(&server_fd);
  init_fd_sets(&master, &read_fds, server_fd);
  server_loop(&master, &read_fds, server_fd);

  return 0;
}
