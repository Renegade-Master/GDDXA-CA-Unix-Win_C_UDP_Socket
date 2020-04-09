//
// Created by Rory Ryan on 09/04/2020.
//

#include "hdr/hangclient_fork.h"


/**
 * Main Client program for online Hangman game served on forking server
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    int sock, count;
    //struct sockaddr_in serv_addr;
    //struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    char * service = argv[2];
    char * server = argv[1];

    // Create a connected TCP socket
    sock = PassiveTCPClient(server, service);
    if (sock < 0)
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");


    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    /* Send message to the server */
    count = write(sock, buffer, strlen(buffer));

    if (count < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    /* Now read server response */
    bzero(buffer,256);
    count = read(sock, buffer, 255);

    if (count < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%s\n",buffer);
    return 0;
}


/**
 * Constructs & assigns a socket for a TCP Client
 * @param server
 * @param service
 * @return
 */
int PassiveTCPClient(const char *server,const char *service){

    struct addrinfo addrCriteria;                   // Criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_UNSPEC;             // v4 or v6 is OK
    addrCriteria.ai_socktype = SOCK_STREAM;         // Only streaming sockets
    addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

    // Get address(es)
    struct addrinfo *servAddr; // Holder for returned list of server addrs
    int rtnVal = getaddrinfo(server, service, &addrCriteria, &servAddr);
    if (rtnVal != 0)
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

    int sock = -1;
    for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {

        sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock < 0){  //Failed
            continue;
        }

        // Establish the connection to the echo server
        if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0){   //Success
            break;
        }

        close(sock); // Failed
        sock = -1;
    }

    freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo()
    return sock;
}