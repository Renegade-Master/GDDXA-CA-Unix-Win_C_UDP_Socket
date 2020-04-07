#!/bin/bash

# Run this script to compile the Client code,
# creating a binary file in the '/bin' folder

# Check for an existing '/bin' folder
if ! [ -e bin/ ]; then
  mkdir bin
fi

# Clear the existing binary files
if [ -e bin/hangclientFork ]; then
  rm bin/hangclientFork
fi

# Compile the binary, with WARNINGS replaced with ERRORS
gcc src/hangClient_Fork.c -o bin/hangclientFork -std=gnu99 -Werror
