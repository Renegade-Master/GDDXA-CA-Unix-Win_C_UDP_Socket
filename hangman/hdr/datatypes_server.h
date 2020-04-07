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

char* word[] = {
#include "../rsc/words"
};

enum Game_State {
    IN_PROGRESS, WON, LOST
};

#define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
#define MAXPLAYERS 2
#define MAXLIVES 10

#endif //HANGMAN_DATATYPES_SERVER_H
