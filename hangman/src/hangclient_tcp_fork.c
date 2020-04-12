/**
 * Author:  Rory Ryan [K00218864]
 * Date:    2020/03/09
 *
 * This is the Source file for the Client side of the TCP version of the
 * Networked Hangman game.
 *
 */

#include "../hdr/hangclient_tcp_fork.h"


/**
 * Main Client program for online Hangman game served on forking server
 * @param argc  - Number of arguments provided to the terminal
 * @param argv  - Arguments provided
 * @return
 */
int main(int argc, char* argv[]) {
    int sock, count;
    char buffer[MAX_LEN];

    char *server = (argc >= 2) ? argv[1] : "localhost";
    //char *service = (argc == 3) ? argv[2] : (char *)HANGMAN_TCP_FORK_PORT;
    char *service = "1268";

    fprintf(stdout, "\n---\nPassivetTCPClient(server: %s, service: %s)\n---\n",server,service);
    // Create a connected TCP socket

    sock = PassiveTCPClient(server, service);
    if (sock < 0){
        perror("\nSetupTCPClientSocket() failed");
        exit(1);
    }



    printf("Please enter the message(Single Letter This is Hangman :) ): ");
    memset(buffer,'\0', sizeof(buffer));
    buffer[0] = (char) fgetc(stdin);

    /* Send message to the server */
    count = write(sock, buffer, strlen(buffer));

    if (count < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    /* Now read server response */
    memset(buffer,'\0',256);
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
 * @param server    - The server IP address the socket will connect to
 * @param service   - The Socket this client program will communicate through
 * @return
 */
int PassiveTCPClient(const char *server,const char *service){
    printf("Passive TCP Client");
    struct addrinfo addrCriteria;                   // Criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_UNSPEC;             // v4 or v6 is OK
    addrCriteria.ai_socktype = SOCK_STREAM;         // Only streaming sockets
    addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

    // Get address(es)
    struct addrinfo *servAddr; // Holder for returned list of server addrs
    int rtnVal = getaddrinfo(server,service, &addrCriteria, &servAddr);
    if (rtnVal != 0) {
        perror("GetAddrInfo() Failed: ");
        exit(2);
    }

    int sock = -1;
    for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
        printf("\n---\nTesting:\n\nsock = socket(addr->ai_family: %d, addr->ai_socktype: %d, addr->ai_protocol: %d)\n\n"
                ,addr->ai_family,addr->ai_socktype,addr->ai_protocol);
        sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock < 0){  //Failed
            perror("\nSocket Failed\n");
            continue;
        }

        printf("connect(sock: %d, addr->ai_addr: %d, addr->ai_addrlen: %d)\n---\n",sock,addr->ai_addr,addr->ai_addrlen);
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