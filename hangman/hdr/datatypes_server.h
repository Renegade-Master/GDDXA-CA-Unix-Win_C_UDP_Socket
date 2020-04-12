/**
 * Author: Ciaran Bent [K00221230]
 * Date:    2020/04/02
 *
 * This is the Header file for the imports and definitions required by
 * ALL versions of the Server side of the Networked Hangman game.
 *
 */

#ifndef HANGMAN_DATATYPES_SERVER_H
#define HANGMAN_DATATYPES_SERVER_H

#include <stdbool.h>
#include <time.h>



char* word[] = {
#include "../rsc/words"
};

enum Game_State {
    IN_PROGRESS, WON, LOST
};

#define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
#define MAX_PLAYERS 2
#define MAX_LIVES 10
#define MAX_TOTAL_LEN 120

#endif //HANGMAN_DATATYPES_SERVER_H
