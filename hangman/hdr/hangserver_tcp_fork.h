#ifndef HANGMAN_HANGSERVER_TCP_FORK_H
#define HANGMAN_HANGSERVER_TCP_FORK_H


#include <sys/wait.h>
#include "datatypes_all.h"
#include "datatypes_server.h"
#include "datatypes_tcp.h"
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include <stdarg.h>



void reaper(int);
int passivesock(const char *service,const char *transport,int qlen);
int passiveTCP(const char *, int);

extern int errno;
typedef unsigned short u_short;
u_short portbase = 0;
int errexit(const char *format, ...);

#endif //HANGMAN_HANGSERVER_TCP_FORK_H
