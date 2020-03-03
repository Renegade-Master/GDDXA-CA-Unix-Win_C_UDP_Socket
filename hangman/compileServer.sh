#!/bin/bash

# Run this script to compile the Server code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangmanServer ]; then
  rm bin/hangmanServer
fi

# Compile the binary, with WARNINGS replaced with ERRORS
gcc src/hangserver.c -o bin/hangmanServer -std=gnu99 -Werror
