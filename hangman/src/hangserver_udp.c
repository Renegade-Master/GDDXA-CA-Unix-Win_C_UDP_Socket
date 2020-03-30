//
// Created by UnityDev on 09/03/2020.
//

#include "../hdr/hangserver_udp.h"

// ToDo: Fill this file out
void play_hangman(int in, int out) {
    fprintf(stdout, "Playing Hangman\n");
}

int main() {
    int                udp_sock, fd, client_len;
    struct sockaddr_in serv_addr, cli_addr;

    srand((int) time((long*) 0)); // randomize the seed

    // Create the UDP Socket
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0); //0 or IPPROTO_UDP

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(HANGMAN_UDP_PORT);

    // Bind the Server socket to an address
    if (bind(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding Datagram Socket Failed\n");
        exit(2);
    }
    fprintf(stdout, "UDP Server Socket Created\n");

    listen(udp_sock, 5);

    // Handle connections
    while (1) {
        client_len = sizeof(cli_addr);
        char mesg[MAXLEN];

        if ((udp_sock = recvfrom(udp_sock, mesg, MAXLEN, 0, (struct sockaddr*) &cli_addr, &client_len)) < 0) {
            perror("Accepting Connection Failed\n");
            exit(3);
        }

        fprintf(stdout, "Client Accepted\n");
        play_hangman(fd, fd);
        close(fd);
    }
}
