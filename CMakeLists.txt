cmake_minimum_required(VERSION 3.14)
project(simple-websocket-chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
option(BUILD_CLIENT "Build WebSocket client" ON)
option(BUILD_SERVER "Build WebSocket server" ON)

include(FetchContent)

# Download ixwebsocket if not installed
FetchContent_Declare(
  ixwebsocket
  GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
  GIT_TAG master
)
FetchContent_MakeAvailable(ixwebsocket)

# Common includes and libraries
include_directories(${ixwebsocket_SOURCE_DIR})
include_directories(${ixwebsocket_SOURCE_DIR}/ixwebsocket)

link_libraries(ixwebsocket ssl crypto pthread z)

# Client
if(BUILD_CLIENT)
  add_executable(client main.cpp)
endif()

# Server
if(BUILD_SERVER)
  add_executable(server servergit.cpp)
endif()
