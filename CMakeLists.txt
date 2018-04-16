cmake_minimum_required(VERSION 3.9)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 11)
set(SOCK_LIB Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h InetAddr.cpp InetAddr.h)
set(MULTI_LIB Multiplexing/MultiSocket.cpp Multiplexing/MultiSocket.h)
set(TCP_FORK_LIB TcpFork/TcpFork.cpp TcpFork/TcpFork.h)

# Tests:
add_executable(SocketTest ${SOCK_LIB} SocketTest.cpp)
add_executable(ServerSocketTest ${SOCK_LIB} ServerSocketTest.cpp)
add_executable(TcpForkTest ${SOCK_LIB} ${TCP_FORK_LIB} TcpFork/TcpForkTest.cpp)
add_executable(TcpForkTestCilent ${SOCK_LIB} ${TCP_FORK_LIB} TcpFork/TcpForkTestClient.cpp)
add_executable(MultiTest ${SOCK_LIB} ${MULTI_LIB} Multiplexing/MultiTest.cpp)
add_executable(EndlessClient ${SOCK_LIB} PublicTests/EndlessClient.cpp)
add_executable(RawSocketTest SocketRaw/RawSocket.h SocketRaw/RawSocket.cpp SocketRaw/RawSocketTest.cpp)

# Executables:
add_executable(NetworkProgramming2Client exp2/client2.cpp ${SOCK_LIB})
add_executable(NetworkProgramming2Server exp2/server2.cpp ${SOCK_LIB})
find_package(Threads REQUIRED)
target_link_libraries(NetworkProgramming2Client Threads::Threads)
target_link_libraries(NetworkProgramming2Server Threads::Threads)
target_link_libraries(MultiTest Threads::Threads)

