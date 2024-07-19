cmake_minimum_required(VERSION 3.22.1)


project(HanaSim CXX)

#export OMP_PREFIX=$(brew --prefix libomp)
#set(ENV{OMP_PREFIX} $(brew --prefix libomp))

FILE(GLOB Sources src/*.cpp)
FILE(GLOB Bots bots/*.cpp)
FILE(GLOB Agents agents/*.cpp)
set (SOURCES ${Sources} ${Bots} ${Agents})

FILE(GLOB Headers include/*.h)
set (HEADERS ${Headers})


#set(HOMEBREW_PREFIX "/usr/local"
#        CACHE PATH "Path to Homebrew installation")

#find_package(Python3 COMPONENTS Intrepreter Development NumPy)
#include_directories(${Python3_INCLUDE_DIRS} include)
#include_directories(/usr/local/include)
#link_directories(/usr/local/lib)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

#target_link_libraries(${CMAKE_PROJECT_NAME} ${Python3_LIBRARIES})
#target_link_libraries(${CMAKE_PROJECT_NAME} numpy)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


#set (LDFLAGS "${LDFLAGS} -L/opt/homebrew/lib")
#set (CPPFLAGS "${CPPFLAGS} -I/opt/homebrew/include")
#CC=clang++
#FLAGS=-g -Wall -std=c++17 -arch arm64 -lomp
#LDFLAGS=-L/opt/homebrew/lib
#CPPFLAGS=-I/opt/homebrew/include
