cmake_minimum_required(VERSION 3.9)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 11)

add_executable(NetworkProgramming2Client client2.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h InetAddr.cpp InetAddr.h)
add_executable(NetworkProgramming2Server server2.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h InetAddr.cpp InetAddr.h)
add_executable(NetworkProgrammingTest test.cpp)
find_package(Threads REQUIRED)
target_link_libraries(NetworkProgramming2Client Threads::Threads)
target_link_libraries(NetworkProgramming2Server Threads::Threads)