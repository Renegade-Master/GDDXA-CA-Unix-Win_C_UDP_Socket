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

char* word[] = {
#include "../rsc/words"
};

#define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
#define MAXPLAYERS 2

#endif //HANGMAN_DATATYPES_SERVER_H
