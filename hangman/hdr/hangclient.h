#ifndef HANGCLIENT_H
#define HANGCLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINESIZE 80
#define HANGMAN_TCP_GENERIC_PORT 1168 // Define this in the Specific Header

int main(int argc, char* argv[]);

#endif // HANGCLIENT_H