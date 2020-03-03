#ifndef HANGSERVER_H
#define HANGSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

void play_hangman(int in, int out);

#endif // HANGSERVER_H
