project(HawChat)
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
file (GLOB SRC_LIST "*.cpp"  "*.h" "include/*.h")

set (COMMON_SRC socket.cpp socket.h common.h channel.cpp
    channel.h connection.cpp connection.h subject.cpp subject.h
    message.h observer.h queue.h inputargreader.h serialization.h
    testserialization.cpp testserialization.h logger.h logger.cpp)

set(CLIENT_SRC ${COMMON_SRC} client.cpp client.h main.cpp)
set(SERVER_SRC ${COMMON_SRC} server.cpp server.h main.cpp user.h user.cpp)

set(CLIENT_TARGET ${PROJECT_NAME}_Client)
set(SERVER_TARGET ${PROJECT_NAME}_Server)
#aux_source_directory(. SRC_LIST)
add_executable(${CLIENT_TARGET} ${CLIENT_SRC})
add_executable(${SERVER_TARGET} ${SERVER_SRC})
target_compile_definitions(${SERVER_TARGET} PRIVATE _SERVER=1)
target_compile_definitions(${CLIENT_TARGET} PRIVATE _CLIENT=1)
