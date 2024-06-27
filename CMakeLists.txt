cmake_minimum_required(VERSION 3.23)


project(HanaSim CXX)

#export OMP_PREFIX=$(brew --prefix libomp)
#set(ENV{OMP_PREFIX} $(brew --prefix libomp))

FILE(GLOB Sources src/*.cpp)
FILE(GLOB Bots bots/*.cpp)
FILE(GLOB Agents agents/*.cpp)
set (SOURCES ${Sources} ${Bots} ${Agents})

FILE(GLOB Headers include/*.h)
set (HEADERS ${Headers})


add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#set (LDFLAGS "${LDFLAGS} -L/opt/homebrew/lib")
#set (CPPFLAGS "${CPPFLAGS} -I/opt/homebrew/include")
#CC=clang++
#FLAGS=-g -Wall -std=c++17 -arch arm64 -lomp
#LDFLAGS=-L/opt/homebrew/lib
#CPPFLAGS=-I/opt/homebrew/include
