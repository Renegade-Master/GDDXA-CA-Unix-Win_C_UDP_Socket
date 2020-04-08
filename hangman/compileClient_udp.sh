#!/bin/bash

# Run this script to compile the Client code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangmanClient_udp ]; then
  rm bin/hangmanClient_udp
fi

# Compile the binary, with WARNINGS replaced with ERRORS
<<<<<<< Updated upstream
gcc src/hangclient_udp.c -o bin/hangmanClient_udp -std=gnu99 -Wall -Wextra -Werror -D_FORTIFY_SOURCE=1
=======
gcc src/hangclient_udp.c -o bin/hangmanClient_udp -std=gnu99 -Wall -Wextra -Werror -D_FORTIFY_SOURCE=1
>>>>>>> Stashed changes
