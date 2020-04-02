/**
 * Author: Ciaran Bent [K00221230]
 * Date:    2020/04/02
 *
 * This is the Header file for the imports and definitions required by
 * the UDP versions of the Networked Hangman game.
 *
 */

#ifndef HANGMAN_DATATYPES_UDP_H
#define HANGMAN_DATATYPES_UDP_H

#define HANGMAN_UDP_PORT 1337

struct udp_data {
    int client_id;
    char message[MAXLEN + 1];
};

#endif //HANGMAN_DATATYPES_UDP_H
