cmake_minimum_required(VERSION 3.9)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 11)
set(SOCK_LIB Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h InetAddr.cpp InetAddr.h)

add_executable(NetworkProgramming main.cpp ${SOCK_LIB})
add_executable(SocketTest ${SOCK_LIB} SocketTest.cpp)
add_executable(ServerSocketTest ${SOCK_LIB} ServerSocketTest.cpp)
add_executable(RawSocketTest SocketRaw/RawSocket.h SocketRaw/RawSocket.cpp SocketRaw/RawSocketTest.cpp)