/* Network server for hangman game */
/* File: hangserver.c */

#include "../hdr/hangserver.h"

extern time_t time();


# define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
# define MAXLEN 80 // Maximum size in the word of any String
# define HANGMAN_TCP_PORT 1066
int maxlives = 12;


void testGame(int in,int out){
	
	printf("Playing Test Connection");
	exit(0);
}


int main() {
    int	sock, fd, client_len, childProcCount;
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
	childProcCount = 0;
    while (1) {
        client_len = sizeof(client);
	    //Accept Connection "FORK HERE!!"
        if ((fd    = accept(sock, (struct sockaddr*) &client, &client_len)) < 0) {
            perror("accepting connection");
            //exit(3);
        }
        pid_t pid = fork();
        
		if(pid < 0){
			perror("Fork() Failed");
		}
        if(pid==0){ //TODO Gamify with Hangman
            perror("Fork Connection Accepted");
            //play_hangman(fd,fd);
			testGame(fd,fd);
			exit(0);
        }
        
		//Increment Child Tracker
		printf("Child Prrocess ID = %d\n" ,pid);
		close(fd);
		childProcCount++;
		//clean up zombies
		while(childProcCount){
			pid = waitpid((pid_t) -1 ,NULL,WNOHANG);
			if(pid < 0){
				perror("waitpid() failed");
			}
			else if(pid ==0){
				perror("No Zombies, break");
				break;
			}
			else{
				perror("Zombie terminated");
				childProcCount--;
			}
		}
        
        //play_hangman(fd, fd);
        close(fd);
    }
}

