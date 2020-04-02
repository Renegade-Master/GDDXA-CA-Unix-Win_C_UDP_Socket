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
#include <sys/wait.h>

extern time_t time();

char* word[] = {
#include "../rsc/words"
};

#define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
#define MAXLEN 80 // Maximum size in the word of any String
#define HANGMAN_TCP_GENERIC_PORT 1066 // Define this in the Specific Header

#endif // HANGSERVER_H
