/**
 * Author: Ciaran Bent [K00221230]
 * Date:    2020/03/09
 *
 * This is the Source file for the Client side of the UDP version of the
 * Networked Hangman game.
 *
 */

#include "../hdr/hangclient_udp.h"


/**
 * play_hangman() function is used to handle playing the Networked
 *  Hangman game.
 *  ToDo: Fill this function out
 */
void play_hangman(int sock, struct sockaddr* serv_addr, socklen_t serv_len, char cli_id[IDLEN]) {
    fprintf(stdout, "Playing Hangman as Client #%s\n", cli_id);
}


/**
 * test_connection() function is used to verify that a connection can be
 *  made to a Server.
 * @param sock       - The Server socket to Send/Receive to/from
 * @param serv_addr  - The address of the remote Server
 * @param serv_len   - The length of the Server Address Structure
 */
void test_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len) {
    int  count;
    char i_line[MAXLEN + 1];
    char o_line[MAXLEN];

    // Zero the data out
    bzero(&i_line, sizeof(i_line));
    bzero(&o_line, sizeof(o_line));

    fprintf(stdout, "Testing Connection\n");

    fprintf(stdout, ">>");
    while (fgets(o_line, MAXLEN, stdin) != NULL) {
        fprintf(stdout, "Sending: %s\n", o_line);

        // Send the data to the Server
        sendto(sock, o_line, strlen(o_line), 0, serv_addr, serv_len);

        // Receive a reply from the Server
        count = recvfrom(sock, i_line, MAXLEN, 0, NULL, NULL);

        i_line[count] = 0;
        fprintf(stdout, "Received: %s\n---\n", i_line);

        bzero(&i_line, sizeof(i_line));
        bzero(&o_line, sizeof(o_line));

        fprintf(stdout, "\n>>");
    }
}


/**
 * setup_connection() function is used to receive a number from the
 *  Server to use when sending any data so that the Server knows which
 *  Client this is.
 * @param sock
 * @param serv_addr
 * @param serv_len
 */
void setup_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len) {
    int  count;
    char id_request[IDLEN + 1];
    char id_response[IDLEN];

    // Zero the data out
    bzero(&id_request, sizeof(id_request));
    bzero(&id_response, sizeof(id_response));

    // Assign request character
    strlcpy(id_request, "-1", IDLEN);

    fprintf(stdout, "Sending: %s\n", id_request);

    // Send the data to the Server
    sendto(sock, id_request, IDLEN, 0, serv_addr, serv_len);

    // Receive a reply from the Server
    count = recvfrom(sock, id_response, IDLEN, 0, serv_addr, &serv_len);

    // Check the received data for errors
    if (count < 0) {
        perror("Receiving from Server Socket Failed\n");
        exit(3); // Error Condition 03
    }

    //id_response[count] = '\0';
    fprintf(stdout, "Received: %s\n---\n", id_response);

    bzero(&id_request, sizeof(id_request));

    play_hangman(sock, (struct sockaddr*) &serv_addr, sizeof(*serv_addr), id_response);
}


/**
 * main() function is the main runtime function of the UDP Client.
 *  It connects to a Server, and begins processes to commence a game of
 *  Networked Hangman.
 * @param argc  - Count of arguments
 * @param argv  - Command Line arguments [Server-IP Server-Port]
 * @return
 */
int main(int argc, char* argv[]) {
    int                udp_sock;
    struct sockaddr_in serv_addr; // Server's address assembled here
    struct hostent* host_info;
    char          * server_name;

    struct timespec tp;
    srand((int) clock_gettime(CLOCK_MONOTONIC, &tp));

    server_name = (argc == 2) ? argv[1] : "localhost";

    host_info = gethostbyname(server_name);
    if (host_info == NULL) {
        perror("Unknown Host\n");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = host_info->h_addrtype;
    memcpy((char*) &serv_addr.sin_addr, host_info->h_addr, host_info->h_length);
    serv_addr.sin_port = htons(HANGMAN_UDP_PORT);

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0); //0 or IPPROTO_UDP

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(2);
    }

    fprintf(stdout, "UDP Client Socket Created\n");

    //test_connection(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    setup_connection(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
}
