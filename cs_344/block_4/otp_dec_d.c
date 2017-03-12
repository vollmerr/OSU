//////////////////////////////////////////////////////////////
//  file:     otp_enc_d.c
//  author:   Ryan Vollmer
//  purpose:  Daemon for dencrypting messages. Works in conjunction
//              with otp_dec
//  usage:    otp_dec_d <port>
//////////////////////////////////////////////////////////////
#include "headers.h"

int main(int argc, char *argv[]) {
  // TODO: check inputs
  // TODO: init 5 ssockets
  // TODO: bind - if err exit, print but continue if bad input, etc
  // TODO: output to stdout
  // TODO: listen on port assigned from args
  // TODO: call accept when conection made, generate socket for actual connect
  // TODO: create new process
  // TODO: ake sure communicating with otp_enc
  // TODO: get text and key from otp_dec via socket
  // TODO: encrypt message
  // TODO: send back encypted to otp_enc
  // TODO: maintain limit of 5 sockets
  return EXIT_SUCCESS;
}
