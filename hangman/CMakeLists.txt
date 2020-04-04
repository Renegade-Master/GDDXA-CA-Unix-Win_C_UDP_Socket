# Set the minimum required version of CMAKE
cmake_minimum_required(VERSION 3.14)

# Set the Project Name
project(HANGMAN_C)

# Set the compiler to GNU99
set_property(TARGET PROPERTY C_STANDARD GNU99)

# Set the desired Compile Flags
set(GCC_COMPILE_FLAGS
        -Wall                   # Enable "All" Warnings
        -Wextra                 # Enable "Extra" Warnings not covered by "All"
        -Werror                 # All Warnings are treated as Errors
        -D_FORTIFY_SOURCE=1)    # Enable Bound Checking of Memory Sensitive Operations

# Add the ROOT Directory
include_directories(.)

# ToDo: Rename the source files as we add new ones.

# Source files for the TCP Fork Application
set(TCP_FORK_CLIENT_SOURCE_FILES
        src/hangclient.c)
set(TCP_FORK_SERVER_SOURCE_FILES
        src/hangserver_Fork.c)

# Source files for the TCP Select Application
set(TCP_SELECT_CLIENT_SOURCE_FILES
        src/hangclient.c)
set(TCP_SELECT_SERVER_SOURCE_FILES
        src/hangserver.c)

# Source files for the UDP Application
set(UDP_CLIENT_SOURCE_FILES
        src/hangclient_udp.c)
set(UDP_SERVER_SOURCE_FILES
        src/hangserver_udp.c)

# Build instructions for the TCP Fork Application
add_executable(hangman_tcp_fork_client ${TCP_FORK_CLIENT_SOURCE_FILES})
add_executable(hangman_tcp_fork_server ${TCP_FORK_SERVER_SOURCE_FILES})

# Build instructions for the TCP Select Application
add_executable(hangman_tcp_select_client ${TCP_SELECT_CLIENT_SOURCE_FILES})
add_executable(hangman_tcp_select_server ${TCP_SELECT_SERVER_SOURCE_FILES})

# Build instructions for the UDP Application
add_executable(hangman_udp_client ${UDP_CLIENT_SOURCE_FILES})
add_executable(hangman_udp_server ${UDP_SERVER_SOURCE_FILES})
