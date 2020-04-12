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
 * play_hangman() function is used to handle serving the Networked
 *  Hangman game.
 * @param sock              - The Server socket to Send/Receive to/from
 * @param cli_addrs         - The address(es) of the remote Client(s)
 * @param cli_len           - The length of the Client Address Structure
 * @param connected_clients - The number of connected Clients
 */
void play_hangman(int sock, struct sockaddr_in* cli_addrs, socklen_t cli_len, const int* connected_clients) {
    fprintf(stdout, "\n---\nPlaying Hangman\n");

    // Set up the game
    ssize_t count;
    char* whole_word,
            part_word[MAX_LEN],
            outbuf[MAX_TOTAL_LEN];
    bool good_guess;
    size_t word_length;
    char hostname[MAX_LEN];
    char guess[GUESS_LEN];
    int lives;
    int clients_in_play = *connected_clients;
    enum Game_State game_state = IN_PROGRESS;

    // Zero out all data before starting
    memset(&count, '\0', sizeof(count));
    memset(&whole_word, '\0', sizeof(whole_word));
    memset(&part_word, '\0', sizeof(part_word));
    memset(&outbuf, '\0', sizeof(outbuf));
    memset(&guess, '\0', sizeof(guess));
    memset(&lives, '\0', sizeof(lives));
    memset(&good_guess, '\0', sizeof(good_guess));
    memset(&word_length, '\0', sizeof(word_length));
    memset(&hostname, '\0', sizeof(hostname));

    fprintf(stdout, "\nThere are %d Clients in play\n", clients_in_play);

    // Pick a word at random from the list
    whole_word = word[random() % NUM_OF_WORDS];
    word_length = strlen(whole_word);
    lives = MAX_LIVES;
    fprintf(stdout, "\nServer chose hangman word %s", whole_word);

    // Ensure no letters are guessed Initially
    for (int j = 0; j < (int) word_length; j++) {
        part_word[j] = '-';
    }

    // Null-terminate the String
    part_word[word_length] = '\0';
    fprintf(stdout, "\nWordWhle: %s", whole_word);
    fprintf(stdout, "\nWordPart: %s", part_word);

    // Get the Human Readable name of this host
    gethostname(hostname, MAX_LEN);

    // Client currently being handled
    struct sockaddr* cli_addr = NULL;

    // Introduce each Client
    for (int i = 0; i < clients_in_play; i++) {
        // Set the current Client
        cli_addr = (struct sockaddr*) &cli_addrs[i];

        memset(&outbuf, '\0', sizeof(outbuf));
        sprintf(outbuf, "%s", hostname);
        sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

        // Check that there were no errors with sending the data
        if (count < 0) {
            perror("Sending to Client Socket Failed\n");
            exit(4); // Error Condition 04
        }

        memset(&outbuf, '\0', sizeof(outbuf));
        snprintf(outbuf, MAX_TOTAL_LEN, "\n\tWord: %s\n\tLives: %hu", part_word, (unsigned short) lives);
        sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

        // Check that there were no errors with sending the data
        if (count < 0) {
            perror("Sending to Client Socket Failed\n");
            exit(4); // Error Condition 04
        }
    }

    // Loop until the game is WON
    while (game_state == IN_PROGRESS) {
        good_guess = false;

        // Loop for each Client
        for (int i = 0; i < clients_in_play; i++) {
            fprintf(stdout, "\nServing Client %d", i);

            // Set the current Client
            cli_addr = (struct sockaddr*) &cli_addrs[i];

            // Inform the Client that it's their turn
            memset(&outbuf, '\0', sizeof(outbuf));
            sprintf(outbuf, "%d", (i + 1));
            sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

            // Check that there were no errors with sending the data
            if (count < 0) {
                perror("Sending to Client Socket Failed\n");
                exit(4); // Error Condition 04
            }

            // Send the current state of the game to the Client
            memset(&outbuf, '\0', sizeof(outbuf));
            snprintf(outbuf, MAX_TOTAL_LEN, "\n\tWord: %s\n\tLives: %hu", part_word, (unsigned short) lives);
            sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

            // Check that there were no errors with sending the data
            if (count < 0) {
                perror("Sending to Client Socket Failed\n");
                exit(4); // Error Condition 04
            }

            // Get a letter from player guess
            count = recvfrom(sock, guess, MAX_LEN, 0, cli_addr, &cli_len);

            // Check the received data for errors
            if (count < 0) {
                perror("Receiving from Client Socket Failed\n");
                exit(3); // Error Condition 03
            }

            // Evaluate the Client's guess
            for (int j = 0; j < (int) word_length; j++) {
                if (guess[0] == whole_word[j]) {
                    good_guess = true;
                    part_word[j] = whole_word[j];
                }
            }

            // If the guess was bad, subtract from the Lives counter
            if (!good_guess) {
                lives--;
            }

            // If the whole word has been guessed
            if (strcmp(whole_word, part_word) == 0) {
                game_state = WON; // User Won

                // Let the Client(s) know they WON
                memset(&outbuf, '\0', sizeof(outbuf));
                sprintf(outbuf, "%s", "#GAMEOVER");
                sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

                // Check that there were no errors with sending the data
                if (count < 0) {
                    perror("Sending to Client Socket Failed\n");
                    exit(4); // Error Condition 04
                }

            } else if (lives == 0) {
                game_state = LOST; // User Lost

                // Let the Client(s) know they LOST
                memset(&outbuf, '\0', sizeof(outbuf));
                sprintf(outbuf, "%s", "#GAMEOVER");
                sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

                // Check that there were no errors with sending the data
                if (count < 0) {
                    perror("Sending to Client Socket Failed\n");
                    exit(4); // Error Condition 04
                }

                strcpy(part_word, whole_word); // User Show the word
            }
        }
    }

    // Send ENDGAME message
    for (int i = 0; i < clients_in_play; i++) {
        // Set the current Client
        cli_addr = (struct sockaddr*) &cli_addrs[i];

        sendto(sock, outbuf, strlen(outbuf), 0, cli_addr, cli_len);

        // Check that there were no errors with sending the data
        if (count < 0) {
            perror("Sending to Client Socket Failed\n");
            exit(4); // Error Condition 04
        }
    }
}


/**
 * test_connection() function is used to verify that a connection can be
 *  made to a Client.
 * @param sock      - The Server socket to Send/Receive to/from
 * @param cli_addr  - The address of the remote Client
 * @param cli_len   - The length of the Client Address Structure
 */
void test_connection(int sock, struct sockaddr* cli_addr, socklen_t cli_len) {
    ssize_t count;
    char i_line[MAX_LEN];

    fprintf(stdout, "Testing Connection\n");

    do {
        memset(&i_line, '\0', sizeof(i_line));
        fprintf(stdout, "---\nAwaiting data on Socket %d...\n\n", sock);

        // Receive data from the Client Socket
        count = recvfrom(sock, i_line, MAX_LEN, 0, cli_addr, &cli_len);
        i_line[count] = '\0';

        // Check the received data for errors
        if (count < 0) {
            perror("Receiving from Client Socket Failed\n");
            exit(3); // Error Condition 03
        }

        // Print the received message to the screen
        fprintf(stdout, "Messg Received: %s", i_line);

        // Send data to the Client Socket
        count = sendto(sock, i_line, MAX_LEN, 0, cli_addr, cli_len);

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
 * @param cli_count - The numerical identifier for this Client
 */
void setup_connections(int sock, struct sockaddr* cli_addr, socklen_t cli_len, const int* cli_count) {
    ssize_t count;
    int client_id;
    char id_request[ID_LEN];
    char id_response[ID_LEN];

    // Zero out data
    memset(&count, '\0', sizeof(count));
    memset(&client_id, '\0', sizeof(client_id));
    memset(&id_request, '\0', sizeof(id_request));
    memset(&id_response, '\0', sizeof(id_response));
    memset(cli_addr, '\0', sizeof(cli_len));

    fprintf(stdout, "\nSetting Up New Client\n");
    fprintf(stdout, "\nAwaiting request on Socket %d...\n", sock);

    // Receive data from the Client Socket
    count = recvfrom(sock, id_request, ID_LEN, 0, cli_addr, &cli_len);

    // Check the received data for errors
    if (count < 0) {
        perror("Receiving from Client Socket Failed\n");
        exit(3); // Error Condition 03
    }

    // Print the received message to the screen
    fprintf(stdout, "\nMessg Received: %s", id_request);
    client_id = (*cli_count) + 1;

    // Assign a Client ID to the Client if it is new
    if (strcmp(id_request, "-1\0") == 0) {
        fprintf(stdout, "\nClient is new.  Assigning ID: %d", client_id);
        sprintf(id_response, "%d", client_id); // Convert the int to char*
    } else {
        fprintf(stdout, "\nClient already assigned ID");
    }

    // Send data to the Client Socket
    count = sendto(sock, id_response, ID_LEN, 0, cli_addr, cli_len);

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
 * @param argc  - The count of cmdline arguments
 * @param argv  - The cmdline arguments, the number of Clients to
 *  connect in this case.
 * @return      - Exit Status
 */
int main(int argc, char* argv[]) {
    // Set the `max_players` to the cmdline option, or MAX_PLAYERS
    int max_players = (argc == 2) ? (int) strtol(argv[1], NULL, 10) : MAX_PLAYERS;
    int udp_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addrs[max_players];
    int connected_clients;

    // Zero out Server data
    memset(&serv_addr, '\0', sizeof(serv_addr));
    memset(&udp_sock, '\0', sizeof(udp_sock));
    memset(&connected_clients, '\0', sizeof(connected_clients));

    for (int i = 0; i < max_players; i++) {
        memset(&cli_addrs[i], '\0', sizeof(cli_addrs[i]));
    }

    // Seed the random number generator
    srandom((unsigned int) time(NULL));

    // Create the UDP Socket
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0); //0 or IPPROTO_UDP

    // Error check The Socket
    if (udp_sock < 0) {
        perror("Creating Datagram Socket Failed\n");
        exit(1); // Error Condition 01
    }

    // Build the Server Address manually
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(HANGMAN_UDP_PORT);

    // Bind the Server socket to an address
    if (bind(udp_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding Datagram Socket Failed\n");
        exit(2); // Error Condition 02
    }
    fprintf(stdout, "UDP Server Socket Created\n");

    // Test connections (DEBUG FUNCTION)
    //test_connection(udp_sock, (struct sockaddr*) &cli_addr, sizeof(cli_addr));

    // Accept Clients until all game slots are full
    connected_clients = 0;
    for (int i = 0; i < max_players; i++) {
        fprintf(stdout, "\n---\nCreating Client #%d", connected_clients);
        setup_connections(udp_sock, (struct sockaddr*) &cli_addrs[i], sizeof(cli_addrs[i]), &connected_clients);
        connected_clients++;
    }

    play_hangman(udp_sock, cli_addrs, sizeof(struct sockaddr), &connected_clients);

    // Close the Socket, and exit the program
    close(udp_sock);
    return (0);
}
