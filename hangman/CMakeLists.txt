cmake_minimum_required(VERSION 3.15)
project(hangman C)

set(CMAKE_GNU_STANDARD 99)

include_directories(.)

# ToDo: Rename the source files as we add new ones.

# Source files for the TCP Fork Application
set(TCP_FORK_CLIENT_SOURCE_FILES
        src/hangclient.c)
set(TCP_FORK_SERVER_SOURCE_FILES
        src/hangserver.c)

# Source files for the TCP Select Application
set(TCP_SELECT_CLIENT_SOURCE_FILES
        src/hangclient.c)
set(TCP_SELECT_SERVER_SOURCE_FILES
        src/hangserver.c)

# Source files for the UDP Application
set(UDP_CLIENT_SOURCE_FILES
        src/hangclient.c)
set(UDP_SERVER_SOURCE_FILES
        src/hangserver.c)

# Build instructions for the TCP Fork Application
add_executable(hangman_tcp_fork_client ${TCP_FORK_CLIENT_SOURCE_FILES})
add_executable(hangman_tcp_fork_server ${TCP_FORK_SERVER_SOURCE_FILES})

# Build instructions for the TCP Select Application
add_executable(hangman_tcp_select_client ${TCP_SELECT_CLIENT_SOURCE_FILES})
add_executable(hangman_tcp_select_server ${TCP_SELECT_SERVER_SOURCE_FILES})

# Build instructions for the UDP Application
add_executable(hangman_udp_client ${UDP_CLIENT_SOURCE_FILES})
add_executable(hangman_udp_server ${UDP_SERVER_SOURCE_FILES})
