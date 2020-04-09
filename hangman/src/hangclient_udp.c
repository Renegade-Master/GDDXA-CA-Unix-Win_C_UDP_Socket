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
 * @param sock      - The Client socket to Send/Receive to/from
 * @param serv_addr - The address of the remote Server
 * @param serv_len  - The length of the Server Address Structure
 * @param cli_id    - The ID Tag for this Client
 */
void play_hangman(int sock, struct sockaddr* serv_addr, socklen_t serv_len, char cli_id[ID_LEN]) {
    ssize_t count;
    int round_local;
    char hostname[MAX_LEN];
    char i_line[MAX_LEN];
    char o_guess[GUESS_LEN];
    char temp_guess[GUESS_LEN];

    // Zero out all data before starting
    memset(&count, '\0', sizeof(count));
    memset(&round_local, '\0', sizeof(round_local));
    memset(&hostname, '\0', sizeof(hostname));
    memset(&i_line, '\0', sizeof(i_line));
    memset(&o_guess, '\0', sizeof(o_guess));
    memset(&temp_guess, '\0', sizeof(temp_guess));

    // Get the Human Readable name of this host
    gethostname(hostname, MAX_LEN);

    fprintf(stdout, "Playing Hangman as Client #%s on [%s]\n", cli_id, hostname);

    // Receive the Hostname of the Server
    count = recvfrom(sock, i_line, MAX_LEN, 0, serv_addr, &serv_len);

    // Check the received data for errors
    if (count < 0) {
        perror("Receiving from Server Socket Failed\n");
        exit(4); // Error Condition 04
    }

    fprintf(stdout, "Connected to Server: %s\n", i_line);

    // Receive the initial game state
    memset(&i_line, '\0', sizeof(i_line));
    count = recvfrom(sock, i_line, MAX_LEN, 0, serv_addr, &serv_len);

    // Check the received data for errors
    if (count < 0) {
        perror("Receiving from Server Socket Failed\n");
        exit(4); // Error Condition 04
    }

    fprintf(stdout, "\nInitial Game State:%s", i_line);

    // Play the game
    do {
        do {
            // Receive the current turn from the Server
            memset(&i_line, '\0', sizeof(i_line));
            count = recvfrom(sock, i_line, MAX_LEN, 0, serv_addr, &serv_len);

            // Check the received data for errors
            if (count < 0) {
                perror("Receiving from Server Socket Failed\n");
                exit(4); // Error Condition 04
            }

            if (strcmp(i_line, "#GAMEOVER\0") == 0) { exit(0); }
        } while (strcmp(i_line, cli_id) != 0);

        fprintf(stdout, "\n---\nRound: %d", ++round_local);

        // Receive the current game state
        memset(&i_line, '\0', sizeof(i_line));
        count = recvfrom(sock, i_line, MAX_LEN, 0, serv_addr, &serv_len);

        // Check the received data for errors
        if (count < 0) {
            perror("Receiving from Server Socket Failed\n");
            exit(4); // Error Condition 04
        }

        fprintf(stdout, "\nGame State:%s", i_line);
        memset(&i_line, '\0', sizeof(i_line));

        // Securely retrieve data from the User
        memset(&temp_guess, '\0', sizeof(temp_guess));

        // Don't allow bad characters
        while (!isalpha((unsigned) temp_guess[0])) { // NOLINT (Bug in CLion)
            fprintf(stdout, "\nGuess a LETTER [a-z]\n>>");
            temp_guess[0] = (char) fgetc(stdin);
        }

        // Convert the letter to lowercase
        temp_guess[0] = tolower(temp_guess[0]);

        // Terminate the String and prepare it for sending
        strncat(temp_guess, "\0", GUESS_LEN);
        strncpy(o_guess, temp_guess, GUESS_LEN);

        // Show the User what they typed
        fprintf(stdout, "Guess was: %s", o_guess);

        // Send the data to the Server
        count = sendto(sock, o_guess, GUESS_LEN, 0, serv_addr, serv_len);

        // Check the sent data for errors
        if (count < 0) {
            perror("Sending to Server Socket Failed\n");
            exit(3); // Error Condition 03
        }

    } while (strcmp(i_line, "#GAMEOVER\0") != 0);
}


/**
 * test_connection() function is used to verify that a connection can be
 *  made to a Server.
 * @param sock      - The Client socket to Send/Receive to/from
 * @param serv_addr - The address of the remote Server
 * @param serv_len  - The length of the Server Address Structure
 */
void test_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len) {
    ssize_t count;
    char i_line[MAX_LEN + 1];
    char o_line[MAX_LEN];

    // Zero the data out
    memset(&i_line, '\0', sizeof(i_line));
    memset(&o_line, '\0', sizeof(o_line));

    fprintf(stdout, "Testing Connection\n");

    fprintf(stdout, ">>");
    while (fgets(o_line, MAX_LEN, stdin) != NULL) {
        fprintf(stdout, "Sending: %s\n", o_line);

        // Send the data to the Server
        sendto(sock, o_line, strlen(o_line), 0, serv_addr, serv_len);

        // Receive a reply from the Server
        count = recvfrom(sock, i_line, MAX_LEN, 0, NULL, NULL);

        // Check the received data for errors
        if (count < 0) {
            perror("Receiving from Server Socket Failed\n");
            exit(3); // Error Condition 03
        }

        i_line[count] = 0;
        fprintf(stdout, "Received: %s\n---\n", i_line);

        memset(&i_line, '\0', sizeof(i_line));
        memset(&o_line, '\0', sizeof(o_line));

        fprintf(stdout, "\n>>");
    }
}


/**
 * setup_connection() function is used to receive a number from the
 *  Server to use when sending any data so that the Server knows which
 *  Client this is.
 * @param sock      - The Client socket to Send/Receive to/from
 * @param serv_addr - The address of the remote Server
 * @param serv_len  - The length of the Server Address Structure
 */
void setup_connection(int sock, struct sockaddr* serv_addr, socklen_t serv_len) {
    ssize_t count;
    char id_request[ID_LEN + 1];
    char id_response[ID_LEN];

    // Zero the data out
    memset(&id_request, '\0', sizeof(id_request));
    memset(&id_response, '\0', sizeof(id_response));

    // Assign request character
    strncpy(id_request, "-1", ID_LEN);

    fprintf(stdout, "Sending: %s\n", id_request);

    // Send the data to the Server
    count = sendto(sock, id_request, ID_LEN, 0, serv_addr, serv_len);

    // Check the sent data for errors
    if (count < 0) {
        perror("Sending to Server Socket Failed\n");
        exit(3); // Error Condition 03
    }

    // Receive a reply from the Server
    count = recvfrom(sock, id_response, ID_LEN, 0, serv_addr, &serv_len);

    // Check the received data for errors
    if (count < 0) {
        perror("Receiving from Server Socket Failed\n");
        exit(4); // Error Condition 04
    }

    fprintf(stdout, "Received: %s\n---\n", id_response);

    memset(&id_request, '\0', sizeof(id_request));

    play_hangman(sock, (struct sockaddr*) &serv_addr, sizeof(*serv_addr), id_response);
}


/**
 * main() function is the main runtime function of the UDP Client.
 *  It connects to a Server, and begins processes to commence a game of
 *  Networked Hangman.
 * @param argc  - The count of cmdline arguments
 * @param argv  - The cmdline arguments, the address of the remote
 *  Server
 * @return      - Exit Status
 */
int main(int argc, char* argv[]) {
    int udp_sock;
    struct addrinfo hints;
    struct addrinfo* serv_info;
    struct addrinfo* next_serv_info;
    struct sockaddr_storage serv_addr;
    char* server_name;
    char server_port[6];

    memset(&udp_sock, '\0', sizeof(udp_sock));
    memset(&hints, '\0', sizeof(hints));
    memset(&serv_addr, '\0', sizeof(serv_addr));
    memset(&server_name, '\0', sizeof(server_name));
    memset(&server_port, '\0', sizeof(server_port));

    // Set the Server address to the cmdline option, or LOCALHOST
    server_name = (argc == 2) ? argv[1] : "::1";
    sprintf(server_port, "%d", HANGMAN_UDP_PORT);

    // Convert the IP Address to a Human-Readable format
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // Build the Server Address Structure
    if (getaddrinfo(server_name, server_port, &hints, &serv_info) != 0) {
        perror("getaddrinfo() failed\n");
        exit(1);
    }

    // Attempt to create the Local Socket
    for(next_serv_info = serv_info; next_serv_info != NULL; next_serv_info = next_serv_info->ai_next) {
        if((udp_sock = socket(next_serv_info->ai_family, next_serv_info->ai_socktype, next_serv_info->ai_protocol)) < 0) {
            perror("Bad Result\n");
            continue;
        }

        // Found a suitable address
        serv_info = next_serv_info;
        break;
    }

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(2);
    }

    fprintf(stdout, "UDP Client Socket Created\n");

    //test_connection(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    // setup_connection(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    setup_connection(udp_sock, (struct sockaddr*) serv_info->ai_addr, sizeof(serv_info->ai_addrlen));

    // Tidy up the Socket and close the program
    freeaddrinfo(serv_info);
    close(udp_sock);

    return(0);
}
