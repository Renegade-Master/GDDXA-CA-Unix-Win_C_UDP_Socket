/* Network server for hangman game */
/* File: hangserver.c */

#include "../hdr/hangserver_tcp_fork.h"


extern time_t time();


# define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
# define MAXLEN 80 // Maximum size in the word of any String
int maxlives = 12;


void testGameNoZombie(int in,int out){
	
	printf("\n--Playing Test Connection--\n\n");
	while(1){
		
	}
	exit(0);
}

void testGameZombie(int in,int out){
	
	printf("\n--Playing Test Connection--\n\n");

	exit(0);
}


void play_hangman(int in, int out) {
    fprintf(stdout, "\n--Playing Hangman--");
    fprintf(stdout, "\n--in", in);
    fprintf(stdout, "\n--out", out);
}

int main() {
    int	sock, fd, client_len, childProcCount;
    struct sockaddr_in server, client;

    //Zero out server data
    bzero(&server, sizeof(server));
    bzero(&client,sizeof(client));
    bzero(&sock, sizeof(sock));
    bzero(&fd, sizeof(fd));
    bzero(&client_len, sizeof(client_len));
    bzero(&childProcCount, sizeof(childProcCount));


    srand((int) time((long*) 0)); // randomize the seed

    sock = socket(AF_INET, SOCK_STREAM, 0);//0 or IPPROTO_TCP
    if (sock < 0) { // This error checking is the code Stevens wraps in his Socket Function etc
        perror("\n--creating stream socket--");
        exit(1);
    }

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port        = htons(HANGMAN_TCP_PORT);

    if (bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("\n--binding socket--");
        exit(2);
    }

    listen(sock, 5);
	childProcCount = 0;
    while (1) {
        client_len = sizeof(client);
	    //Accept Connection "FORK HERE!!"
        if ((fd    = accept(sock, (struct sockaddr*) &client, &client_len)) < 0) {
            perror("\n--Error accepting connection--");
            exit(3);
        }
        pid_t pid = fork();
        
		if(pid < 0){
			perror("\n--Fork() Failed--");
		}
        if(pid==0){ //TODO Gamify with Hangman
            perror("\n--Fork Connection Accepted--");
            play_hangman(fd,fd);
			exit(0);
        }
        
		//Increment Child Tracker
		close(fd);
		childProcCount++;


		//clean up zombies
		while(childProcCount){
		    //Get Current State of processes
			pid = waitpid((pid_t) -1 ,NULL,WNOHANG);

			if(pid < 0){
				perror("\n--waitpid() failed No Zombie Found--");
			}
			else if(pid ==0){
				perror("\n--No Zombies, break--");
				break;
			}
			else{
				perror("\n--Zombie terminated--");
				childProcCount--;
			}
		}
        
    }
}

