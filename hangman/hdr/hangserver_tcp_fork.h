#ifndef HANGMAN_HANGSERVER_TCP_FORK_H
#define HANGMAN_HANGSERVER_TCP_FORK_H


#include <sys/wait.h>
#include "datatypes_all.h"
#include "datatypes_server.h"
#include "datatypes_tcp.h"


<<<<<<< HEAD





void reaper();
int passivesock(const char *service,const char *transport,int qlen);
int passiveTCP(const char *, int);

//extern int errno;
typedef unsigned short u_short;
u_short portbase = 0;
int errexit(const char *format, ...);
=======
>>>>>>> parent of 10e2461... 'hangserver_Fork.c'  Added Abstracted Socket Assignmnet Function

#endif //HANGMAN_HANGSERVER_TCP_FORK_H
