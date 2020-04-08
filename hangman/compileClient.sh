#!/bin/bash

# Run this script to compile the Client code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangmanClient ]; then
  rm bin/hangmanClient
fi

# Compile the binary, with WARNINGS replaced with ERRORS
gcc src/hangclient.c -o bin/hangmanClient -std=gnu99 -Wall -Wextra -Werror -D_FORTIFY_SOURCE=1
