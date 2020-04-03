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
 * @param sock      - The Server socket to Send/Receive to/from
 * @param cli_addrs - The addresses of the remote Clients
 * @param cli_len   - The length of the Client Address Structure
 */
void play_hangman(int sock, const void* cli_addrs, socklen_t cli_len) {
    fprintf(stdout, "\n---\nPlaying Hangman\n");
    fprintf(stdout, "\nThere are %lu Clients in play", (sizeof(cli_addrs) / sizeof(cli_len)));
}


/**
 * test_connection() function is used to verify that a connection can be
 *  made to a Client.
 * @param sock      - The Server socket to Send/Receive to/from
 * @param cli_addr  - The address of the remote Client
 * @param cli_len   - The length of the Client Address Structure
 */
void test_connection(int sock, struct sockaddr* cli_addr, socklen_t cli_len) {
    int  count;
    char i_line[MAXLEN];

    fprintf(stdout, "Testing Connection\n");

    do {
        bzero(&i_line, sizeof(i_line));
        fprintf(stdout, "---\nAwaiting data on Socket %d...\n\n", sock);

        // Receive data from the Client Socket
        count = recvfrom(sock, i_line, MAXLEN, 0, cli_addr, &cli_len);
        i_line[count] = '\0';

        // Check the received data for errors
        if (count < 0) {
            perror("Receiving from Client Socket Failed\n");
            exit(3); // Error Condition 03
        }

        // Print the received message to the screen
        fprintf(stdout, "Messg Received: %s", i_line);

        // Send data to the Client Socket
        count = sendto(sock, i_line, count, 0, cli_addr, cli_len);


        // Check that there were no errors with sending the data
        if (count < 0) {
            perror("Sending to Client Socket Failed\n");
            exit(4); // Error Condition 04
        }

        // Print confirmation of the send to the screen
        fprintf(stdout, "Messg Sent: %s", i_line);
    } while (strcmp(i_line, "#quit\0") != 0); // ToDo: Create stop condition that actually works
}


/**
 * setup_connections() function is used to add Clients to the game.
 * @param sock      - The Client socket to Send/Receive to/from
 * @param cli_addr  - The address of the remote Client
 * @param cli_len   - The length of the Client Address Structure
 */
void setup_connections(int sock, struct sockaddr* cli_addr, socklen_t cli_len, const int* cli_count) {
    int  count;
    char id_request[IDLEN];
    char id_response[IDLEN];

    // Zero out data
    bzero(&count, sizeof(count));
    bzero(&id_request, sizeof(id_request));
    bzero(&id_response, sizeof(id_response));

    fprintf(stdout, "\nSetting Up New Client\n");
    fprintf(stdout, "\nAwaiting request on Socket %d...\n", sock);

    // Receive data from the Client Socket
    count = recvfrom(sock, id_request, IDLEN, 0, cli_addr, &cli_len);

    // Check the received data for errors
    if (count < 0) {
        perror("Receiving from Client Socket Failed\n");
        exit(3); // Error Condition 03
    }

    // Print the received message to the screen
    fprintf(stdout, "\nMessg Received: %s", id_request);

    // Assign a Client ID to the Client if it is new
    if (strcmp(id_request, "-1\0") == 0) {
        fprintf(stdout, "\nClient is new.  Assigning ID: %d", *cli_count);
        sprintf(id_response, "%d", *cli_count); // Convert the int to char*
    } else {
        fprintf(stdout, "\nClient already assigned ID");
    }

    // Send data to the Client Socket
    count = sendto(sock, id_response, IDLEN, 0, (struct sockaddr*) cli_addr, cli_len);

    // Check that there were no errors with sending the data
    if (count < 0) {
        perror("Sending to Client Socket Failed\n");
        exit(4); // Error Condition 04
    }

    // Print confirmation of the send to the screen
    fprintf(stdout, "\nMessg Sent: %s\n", id_response);
}


/**
 * main() function is the main runtime function of the UDP Server.
 *  It gathers several Clients and launches the Server for the Networked
 *  Hangman game.
 * @return
 */
int main() {
    int                udp_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addrs[MAXPLAYERS];
    int                connected_clients;

    // Zero out Server data
    bzero(&serv_addr, sizeof(serv_addr));
    bzero(&udp_sock, sizeof(udp_sock));
    bzero(&connected_clients, sizeof(connected_clients));

    for (int i = 0; i < MAXPLAYERS; i++) {
        bzero(&cli_addrs[i], sizeof(cli_addrs[i]));
    }

    // Seed the random number generator
    struct timespec tp;
    srand((int) clock_gettime(CLOCK_MONOTONIC, &tp));

    // Create the UDP Socket
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0); //0 or IPPROTO_UDP

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(1); // Error Condition 01
    }

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
    //test_connection(udp_sock, (struct sockaddr*) &cli_addr, sizeof(cli_addr));

    // Accept Clients until all game slots are full
    connected_clients = 1;
    for (int i = 0; i < MAXPLAYERS; i++) {
        fprintf(stdout, "\n---\nCreating Client #%d", connected_clients);
        setup_connections(udp_sock, (struct sockaddr*) &cli_addrs[i], sizeof(cli_addrs[i]), &connected_clients);
        connected_clients++;
    }

    play_hangman(udp_sock, &cli_addrs, sizeof(struct sockaddr));
}
