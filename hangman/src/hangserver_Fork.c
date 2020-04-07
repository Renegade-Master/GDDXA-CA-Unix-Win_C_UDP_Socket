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


void play_hangman(int sock,struct sockaddr* cli_adr,socklen_t cli_len) {
    fprintf(stdout, "\n--Playing Hangman--");
    fprintf(stdout, "\n--sock: %s", sock);
    fprintf(stdout, "\n--cli_addr", cli_adr);
    fprintf(stdout, "\n--c0li_len", cli_len);
    bool endGame = false;
    int count;
    char i_line[MAXLEN];

    //test Transmission from user
    do{
        //fprintf(stdout,"---\nAwaiting Data%d...\n\n", sock);

        count = recv(sock,i_line,MAXLEN,0);
        i_line[count]= '\0';
        //Did Message Receive Correctly
        if(count<0){
            perror("\n---\nReceive Failed!\n---\n");
            exit(3);
        }

        fprintf(stdout,"\nMessage Reads: %s",i_line);


        count = sendto(sock,i_line,count,0,cli_adr,cli_len);





    }while(!endGame);
}
 int test(int sock){
    while(1){}
}


int main() {
    printf("\nStarted");
    int	sock, ssock, fd, client_len, childProcCount, numOfClients;
    struct sockaddr_in server, client[MAXPLAYERS];
    char *service;

    printf("\nZeroing");
    //Zero out server data
    bzero(&server, sizeof(server));
    bzero(&sock, sizeof(sock));
    bzero(&ssock, sizeof(ssock));
    bzero(&fd, sizeof(fd));
    bzero(&client_len, sizeof(client_len));
    bzero(&childProcCount, sizeof(childProcCount));
    bzero(&numOfClients,sizeof(numOfClients));
    bzero(&service,sizeof(service));

    printf("\npost Zeroing");
    for(int i = 0; i < MAXPLAYERS; i++) {
        bzero(&client[i], sizeof(client[i]));
    }
    service = "1168";

    perror("\n---\nCreating Socket\n---\n");
    sock = passiveTCP(service,5);
    perror("\n---\nCreated Socket\n---\n");
    (void) signal(SIGCHLD, reaper);


    if (sock < 0) { // This error checking is the code Stevens wraps in his Socket Function etc
        perror("\n--creating stream socket--");
        exit(1);
    }

    /*server.sin_family      = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port        = htons(HANGMAN_TCP_PORT);

    if (bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("\n--binding socket--");
        exit(2);
    }*/

    //listen(sock, 5);
	//childProcCount = 0;
    while (1) {
        client_len = sizeof(client[0]);
        printf("\n---\nAccepting?\n---\n");
        ssock = accept(sock,(struct sockaddr *)&server,&client_len);
        if(ssock < 0){
            perror("Accept Failed \n");
            printf("Failure");
        }
        else{
            perror("Accept Succeeded \n");
        }


        switch(fork()){
            case 0: //Child
                close(sock);
                exit(test(ssock));
                break;
            default:
                close(ssock);
                break;
        }

        // Run Hangman
        (void) close(ssock);
        /*client_len = sizeof(client);
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
            printf("%d is a socket\n", fd);
            play_hangman(sock,(struct sockaddr*) &server, sizeof(server));
			exit(0);
        }*/
        
		//Increment Child Tracker
		//close(fd);
		//childProcCount++;


		//clean up zombies
		/*while(childProcCount){
		    //Get Current State of processes
			pid = waitpid((pid_t) -1 ,NULL,WNOHANG);
            printf("%d is a socket\n", pid);


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
		}*/
        
    }
}


//Find and Terminate Zombie Child Process'
void reaper(int siq){

    int status;
    bzero(&status, sizeof(status));

    while (wait3(&status,WNOHANG,(struct rusage *)0)>=0){
        if(status < 0){
            perror("\n--waitpid() failed No Zombie Found--");
        } else if(status == 0){
            perror("\n--No Zombies, break--");
            break;
        }
        else{
            perror("\n--Zombie terminated--");
        }
    }

}

//Passively Assign Socket Connection in TCP
int passiveTCP(const char *service, int qlen){
    printf("PassiveTCP");
    return passivesock(service,"tcp",qlen);
}

//Allocate & bind server socket
int passivesock(const char *service,const char *transport,int qlen) {
    printf("PassiveSock");
    struct servent *pse;        //Pointer to service information entry;
    struct protoent *ppe;        //pointer to protocol information entry
    struct sockaddr_in sin;     //an internet endpoint address
    int s, type;                //socket Descriptor and socket type

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;


    /* Map service name to port number */
    if (pse = getservbyname(service, transport)) {
        sin.sin_port = htons(ntohs((u_short) pse->s_port) + portbase);
    }
    else if ((sin.sin_port = htons((u_short) atoi(service))) == 0) {
        errexit("can't get \"%s\" service entry\n", service);
    }

    /* Map protocol name to protocol number */
    if ((ppe = getprotobyname(transport)) == 0) {
        errexit("can't get \"%s\" protocol entry\n", transport);
    }


    /* Use protocol to choose a socket type */
    if (strcmp(transport, "udp") == 0) {
        type = SOCK_DGRAM;
    }
    else {
        type = SOCK_STREAM;
    }


    /* Allocate a socket */
    s = socket(PF_INET, type, ppe->p_proto);
    if (s < 0) {
        errexit("can't create socket: %s\n", strerror(errno));
    }
    /* Bind the socket */
    if (bind(s, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        errexit("can't bind to %s port: %s\n", service, strerror(errno));
    }

    if (type == SOCK_STREAM && listen(s, qlen) < 0) {
        errexit("can't listen on %s port: %s\n", service,strerror(errno));
    }
    return s;
}

int errexit(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}




