/* Network server for hangman game */
/* File: hangserver.c */

#include "../hdr/hangserver.h"

extern time_t time();

char* word[] = {
#include "../rsc/words"
};

# define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
# define MAXLEN 80 // Maximum size in the word of any String
# define HANGMAN_TCP_PORT 1066
int maxlives = 12;



int main() {
    int                sock, fd, client_len;
    struct sockaddr_in server, client;

    srand((int) time((long*) 0)); // randomize the seed

    sock = socket(AF_INET, SOCK_STREAM, 0);//0 or IPPROTO_TCP
    if (sock < 0) { // This error checking is the code Stevens wraps in his Socket Function etc
        perror("creating stream socket");
        exit(1);
    }

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port        = htons(HANGMAN_TCP_PORT);

    if (bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("binding socket");
        exit(2);
    }

    listen(sock, 5);

    while (1) {
        client_len = sizeof(client);
	//Accept Connection "FORK HERE!!"
        if ((fd    = accept(sock, (struct sockaddr*) &client, &client_len)) < 0) {
            perror("accepting connection");
            exit(3);
        }
        //play_hangman(fd, fd);
        close(fd);
    }
}
