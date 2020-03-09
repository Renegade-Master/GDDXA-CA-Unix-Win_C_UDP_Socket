#!/bin/bash

# Run this script to compile the Server code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangmanServer_Fork ]; then
  rm bin/hangmanServer_Fork
fi

# Compile the binary, with WARNINGS replaced with ERRORS
gcc src/hangserver_Fork.c -o bin/hangmanServer_Fork -std=gnu99 -Werror
