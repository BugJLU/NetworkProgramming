cmake_minimum_required(VERSION 3.9)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 11)

add_executable(NetworkProgramming main.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h InetAddr.cpp InetAddr.h)