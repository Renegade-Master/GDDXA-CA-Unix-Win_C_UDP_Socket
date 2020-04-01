/**
 * Author: Ciaran Bent [K00221230]
 * Date:    2020/03/09
 *
 * This is the Source file for the Server side of the UDP version of the
 * Networked Hangman game.
 *
 */

#include "../hdr/hangserver_udp.h"


/**
 * play_hangman() function is used to handle playing the Networked
 *  Hangman game.
 *  ToDo: Fill this function out
 * @param sock      - The Client socket to Send/Receive to/from
 * @param cli_addr  - The address of the remote Client
 * @param cli_len   - The length of the Client Address Structure
 */
void play_hangman(int sock, struct sockaddr *cli_addr, socklen_t cli_len) {
    fprintf(stdout, "Playing Hangman\n");
}


/**
 * test_connection() function is used to verify that a connection can be
 *  made to a Client.
 * @param sock      - The Client socket to Send/Receive to/from
 * @param cli_addr  - The address of the remote Client
 * @param cli_len   - The length of the Client Address Structure
 */
void test_connection(int sock, struct sockaddr *cli_addr, socklen_t cli_len) {
    int count;
    char i_line[MAXLEN];

    fprintf(stdout, "Testing Connection\n");

    do {
        bzero(&i_line, sizeof(i_line));
        fprintf(stdout, "---\nAwaiting data on Socket %d...\n\n", sock);

        // Receive data from the Client Socket
        count = recvfrom(sock, i_line, MAXLEN, 0, cli_addr, &cli_len);
        i_line[count] = 0;

        // Check the received data for errors
        if (count < 0) {
            perror("Receiving from Client Socket Failed\n");
            exit(3); // Error Condition 03
        }

        // Print the received message to the screen
        fprintf(stdout, "Messg Received: %s", i_line);
        fprintf(stdout, "Bytes Received: %d\n\n", count);

        // Send data to the Client Socket
        count = sendto(sock, i_line, count, 0, cli_addr, cli_len);

        // Check that there were no errors with sending the data
        if (count < 0) {
            perror("Sending to Client Socket Failed\n");
            exit(4); // Error Condition 04
        }

        // Print confirmation of the send to the screen
        fprintf(stdout, "Messg Sent: %s", i_line);
        fprintf(stdout, "Bytes sent: %d\n\n", count);
    } while(strcmp(i_line, "#quit") != 0); // ToDo: Create stop condition that actually works
}


/**
 * main() function is the main runtime function of the UDP Server.
 *  It gathers several Clients and launches the Server for the Networked
 *  Hangman game.
 * @return
 */
int main() {
    int                udp_sock;
    struct sockaddr_in serv_addr, cli_addr;

    srand((int) time((long*) 0)); // randomize the seed

    // Create the UDP Socket
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0); //0 or IPPROTO_UDP

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(1); // Error Condition 01
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(HANGMAN_UDP_PORT);

    // Bind the Server socket to an address
    if (bind(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding Datagram Socket Failed\n");
        exit(2); // Error Condition 02
    }
    fprintf(stdout, "UDP Server Socket Created\n");

    // Handle connections
    // ToDo: Accept X number of Clients for playing Hangman with
    test_connection(udp_sock, (struct sockaddr*) &cli_addr, sizeof(cli_addr));
    // play_hangman(udp_sock, (struct sockaddr*) &cli_addr, sizeof(cli_addr));
}
