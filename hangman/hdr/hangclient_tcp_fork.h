/**
 * Author:  Rory Ryan [K00218864]
 * Date:    2020/03/09
 *
 * This is the Header file for the Client side of the TCP version of the
 * Networked Hangman game.
 *
 */

#ifndef HANGMAN_HANGSERVER_FORK_H
#define HANGMAN_HANGSERVER_FORK_H

#include "datatypes_all.h"
#include "datatypes_client.h"
#include "datatypes_tcp.h"


void play_hangman(int , struct sockaddr*, socklen_t , char);

void test_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len);

void setup_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len);

int PassiveTCPClient(const char *,const char *);



#endif //HANGMAN_HANGSERVER_FORK_H
