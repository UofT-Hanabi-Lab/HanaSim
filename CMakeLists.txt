cmake_minimum_required(VERSION 3.22.1)

project(HanaSim CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
#include_directories(/opt/homebrew/opt/pybind11/lib/python3.13/site-packages/pybind11/include)
#include_directories(/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13)
find_package(pybind11 REQUIRED)

# Get Python site-packages directory
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import site; print(site.getsitepackages()[0])"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

FILE(GLOB Sources src/*.cpp)
FILE(GLOB Bots bots/*.cpp)
FILE(GLOB Agents agents/*.cpp)
set (SOURCES ${Sources} ${Bots} ${Agents})

FILE(GLOB Headers include/*.h)
set (HEADERS ${Headers})

find_package(OpenMP)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
pybind11_add_module(hana_sim ${SOURCES})
target_link_libraries(hana_sim PRIVATE pybind11::module OpenMP::OpenMP_CXX ${Python3_LIBRARIES})

# Install the module to Python site-packages
install(TARGETS hana_sim 
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
        RUNTIME DESTINATION ${PYTHON_SITE_PACKAGES})

