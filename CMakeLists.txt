cmake_minimum_required(VERSION 3.23)


project(HanaSim CXX)


FILE(GLOB Sources src/*.cpp)
FILE(GLOB Bots bots/*.cpp)
FILE(GLOB Agents agents/*.cpp)
set (SOURCES ${Sources} ${Bots} ${Agents})

FILE(GLOB Headers include/*.h)
set (HEADERS ${Headers}
        include/humanplayer.h)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
