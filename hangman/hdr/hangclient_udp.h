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

#include "datatypes_all.h"
#include "datatypes_client.h"
#include "datatypes_udp.h"

#define GUESSLEN 2

void play_hangman(int sock, struct sockaddr* serv_addr, socklen_t serv_len, char cli_id[IDLEN]);

void test_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len);

void setup_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len);

#endif //HANGMAN_HANGSERVER_UDP_H
