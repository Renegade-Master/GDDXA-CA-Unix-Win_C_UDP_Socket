/**
 * Author:  Rory Ryan [K00218864]
 * Date:    2020/03/09
 *
 * This is the Source file for the Client side of the TCP version of the
 * Networked Hangman game.
 *
 */

#include "../hdr/hangserver_tcp_fork.h"




/**
 * A Function Designed to create a thread that does
 * not become a Zombie for testing purposes
 */
void testGameNoZombie(){

    printf("\n--Playing Test Connection--\n\n");
    while(1){

    }
    exit(0);
}

/**
 *  A Function Designed to create a thread that does
 *  become a Zombie for testing purposes
 */
void testGameZombie(){

    printf("\n--Playing Test Connection--\n\n");

    exit(0);
}

/**
 *  The m,ain hangman game
 * @param sock      - What Socket the hangman game service will be run through
 * @param cli_adr   - The client playing Hangman
 * @param cli_len   - The Length of the Client address
 */
void play_hangman(int sock,struct sockaddr* cli_adr,socklen_t cli_len) {
    fprintf(stdout, "\n--Playing Hangman--");
    fprintf(stdout, "\n--sock: %d", sock);
    fprintf(stdout, "\n--cli_addr: %d", cli_adr->sa_family);
    fprintf(stdout, "\n--c0li_len: %d", cli_len);
    bool endGame = false;
    int count;
    char i_line[MAX_LEN];

    //test Transmission from user
    do{
        //fprintf(stdout,"---\nAwaiting Data%d...\n\n", sock);

        count = recv(sock,i_line,MAX_LEN,0);
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
    int count;
    char buffer[MAX_LEN];
    while(1) {
        memset(&buffer, '\0', sizeof(buffer));

        printf("\nSocket: %d, Waiting for Message: ",sock);
        count = read(sock, buffer, sizeof(buffer));

        if(count<=0){
            perror("\nRead operation failed");
            exit(2);
        }
        printf("\n\nMessage Reads: %s",buffer);
    }
}



/**
 * The Signal Function designed to search for
 * and Terminate Zombie Processses in the main program
 */
void reaper(){

    int status;
    memset(&status,'\0', sizeof(status));

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

/**
 * Call a sokcket build structure based on the protocol provided
 * Currently only calls TCP
 * @param service   - The Socket/Service to be used
 * @param qlen      - The number of Clients in queue to be connected
 * @return
 */
int passiveTCP(int service, int qlen){
    printf("---\nPassiveTCP");
    return passivesock(service,"tcp",qlen);
}

/**
 * Allocate & bind server socket
 * @param service       - The Socket/Service to be used
 * @param transport     - The Transport Protocol being used by this socket
 * @param qlen          - The number of Clients in queue to be connected
 * @return
 */
int passivesock(int service,const char *transport,int qlen) {
    printf("\n---\nPassiveSock");
    //u_short portbase = 0;
    //struct servent *pse;        //Pointer to service information entry;
    struct protoent *ppe;        //pointer to protocol information entry
    struct sockaddr_in sin;     //an internet endpoint address
    int s, type;                //socket Descriptor and socket type

    memset(&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(service);
    printf("\nTransport: \n");
    /* Map service name to port number */
    /*if ((pse = getservbyname((const char *)service, transport))) {
        sin.sin_port = htons(ntohs((u_short) pse->s_port) + portbase);
    }
    else if ((sin.sin_port = htons((u_short) atoi(service))) == 0) {
        perror("Cant get service entry");
    }*/

    /* Map protocol name to protocol number */
    if ((ppe = getprotobyname(transport)) == 0) {
        perror("Cant get protocol entry");
    }


    /* Use protocol to choose a socket type */
    type = SOCK_STREAM;


    /* Allocate a socket */
    s = socket(PF_INET, type, ppe->p_proto);
    if (s < 0) {
        //errexit("can't create socket: %s\n", strerror(errno));
        perror("cant create socket");
        exit(1);
    }
    /* Bind the socket */
    if (bind(s, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        //errexit("can't bind to %s port: %s\n", service, strerror(errno));
        perror("cant bind socket");
        exit(1);
    }

    if (type == SOCK_STREAM && listen(s, qlen) < 0) {
        //errexit("can't listen on %s port: %s\n", service,strerror(errno));
        perror("cant listen on port");
        exit(1);
    }
    return s;
}



/**
 *  Main function of hangserver_tcp_fork(), Builds and deploys server
 *  from variables set in Header files using TCp transport protocol for the socket
 *  Fork() each incoming connection to run on a seperate thread when received
 *  Deploys Signal Function reaper() to check for Orphaned Processes
 * @return
 */
int main() {
    printf("\nStarted");
    int	sock, ssock, fd, client_len, childProcCount, numOfClients;
    struct sockaddr_in server, client[MAX_PLAYERS];
    int service;

    printf("\n---\nZeroing");
    memset(&server,'\0', sizeof(server));
    memset(&sock,'\0', sizeof(sock));
    memset(&ssock,'\0', sizeof(ssock));
    memset(&fd,'\0', sizeof(fd));
    memset(&client_len,'\0', sizeof(client_len));
    memset(&childProcCount,'\0', sizeof(childProcCount));
    memset(&numOfClients,'\0',sizeof(numOfClients));
    memset(&service,'\0',sizeof(service));

    printf("\n---\npost Zeroing\n---\n");
    for(int i = 0; i < MAX_PLAYERS; i++) {
        memset(&client[i],'\0', sizeof(client[i]));
    }
    service = (HANGMAN_TCP_FORK_PORT);

    printf("Creating Socket \n");
    sock = passiveTCP(service,5);
    perror("\n---\nCreated Socket");
    signal(SIGCHLD, reaper);


    if (sock < 0) { // This error checking is the code Stevens wraps in his Socket Function etc
        perror("\n--creating stream socket--");
        exit(1);
    }

    for (int i =0;i < MAX_PLAYERS; i++) {

        client_len = sizeof(client[0]);
        printf("\n---\nAccepting?\n\n");
        ssock = accept(sock,(struct sockaddr *)&server,(socklen_t *)&client_len);

        printf("Sock: %d produced child ssock: %d",sock,ssock);
        if(ssock < 0){
            perror("Accept Failed \n---\n");
            printf("Failure\n---\n");
        }
        else{
            perror("Accept Succeeded\n---\n");
        }


        switch(fork()){
            case 0: //Child
                printf("\n---\nForked new Child Process\nParent: %d\n"
                       "Created Child Process\n New Child: %d",getppid(),getpid());
                close(sock);//Child Doesnt Need Listener Port
                test(ssock);
                break;
            default:
                printf("Failed to Fork new Child Process\n---\n");
                close(ssock);
                break;
        }

        close(ssock);
    }
}