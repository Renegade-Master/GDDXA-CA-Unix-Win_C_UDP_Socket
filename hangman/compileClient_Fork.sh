#!/bin/bash

# Run this script to compile the Client code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangmanClient_Fork ]; then
  rm bin/hangmanClient_Fork
fi

# Compile the binary, with WARNINGS replaced with ERRORS
gcc src/hangclient_Fork.c -o bin/hangmanClient_Fork -std=gnu99 -Werror
