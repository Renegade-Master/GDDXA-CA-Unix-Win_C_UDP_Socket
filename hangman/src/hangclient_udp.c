//
// Created by UnityDev on 09/03/2020.
//

#include "../hdr/hangclient_udp.h"

// ToDo: Fill this file out
void play_hangman(int sock, const struct sockaddr *serv_addr, socklen_t serv_len) {
    int count;
    char i_line[LINESIZE];
    char o_line[LINESIZE];

    fprintf(stdout, "Playing Hangman\n");

    while ((count = read(sock, i_line, LINESIZE)) > 0) {
        write(1, i_line, count);
        count = read(0, o_line, LINESIZE);  //0 = STDIN
        write(sock, o_line, count);
    }
}

int main(int argc, char* argv[]) {
    int                udp_sock, fd;
    struct sockaddr_in serv_addr; // Server's address assembled here
    struct hostent     * host_info;
    char               * server_name;

    server_name = (argc == 2) ? argv[1] : "localhost";

    host_info = gethostbyname(server_name);
    if (host_info == NULL) {
        perror("Unknown Host\n");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = host_info->h_addrtype;
    memcpy((char*) &serv_addr.sin_addr, host_info->h_addr, host_info->h_length);
    serv_addr.sin_port        = htons(HANGMAN_UDP_PORT);
    //Inet_pton(AF_INET, argv[1], &serv_addr.sin_addr); // ToDo: What is this?

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0); //0 or IPPROTO_UDP

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(2);
    }

    fprintf(stdout, "UDP Client Socket Created\n");

    play_hangman(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
}