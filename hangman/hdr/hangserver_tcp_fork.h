#ifndef HANGMAN_HANGSERVER_TCP_FORK_H
#define HANGMAN_HANGSERVER_TCP_FORK_H


#include <sys/wait.h>
#include "datatypes_all.h"
#include "datatypes_server.h"
#include "datatypes_tcp.h"

void reaper();
int passivesock(const char *service,const char *transport,int qlen);
int passiveTCP(const char *, int);

//extern int errno;
typedef unsigned short u_short;

int errexit(const char *format, ...);

#endif //HANGMAN_HANGSERVER_TCP_FORK_H
