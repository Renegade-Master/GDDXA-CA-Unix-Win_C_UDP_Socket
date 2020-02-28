# This script is designed to compile both the Server and the Client
# in one command to make combined compilation easier.

gcc src/hangserver.c -o bin/hangmanServer
gcc src/hangclient.c -o bin/hangmanClient

