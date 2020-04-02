#!/bin/bash

# Run this script to compile the Client code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangclient ]; then
  rm bin/hangclient
fi

# Compile the binary, with WARNINGS replaced with ERRORS
gcc src/hangclient.c -o bin/hangclient -std=gnu99 -Werror
