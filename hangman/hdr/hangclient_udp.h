//
// Created by UnityDev on 09/03/2020.
//

#ifndef HANGMAN_HANGSERVER_UDP_H
#define HANGMAN_HANGSERVER_UDP_H

#include "hangclient.h"

# define HANGMAN_UDP_PORT 1337

void play_hangman(int sock, const struct sockaddr *serv_addr, socklen_t serv_len);

#endif //HANGMAN_HANGSERVER_UDP_H
