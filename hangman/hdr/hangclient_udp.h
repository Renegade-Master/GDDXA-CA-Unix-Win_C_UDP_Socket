/**
 * Author:  Ciaran Bent [K00221230]
 * Date:    2020/03/09
 *
 * This is the Header file for the Client side of the UDP version of the
 * Networked Hangman game.
 *
 */

#ifndef HANGMAN_HANGSERVER_UDP_H
#define HANGMAN_HANGSERVER_UDP_H

#include "hangclient.h"

#define HANGMAN_UDP_PORT 1337

void play_hangman(int sock, const struct sockaddr *serv_addr, socklen_t serv_len);
void test_connection(int sock, const struct sockaddr *serv_addr, socklen_t serv_len);

#endif //HANGMAN_HANGSERVER_UDP_H
