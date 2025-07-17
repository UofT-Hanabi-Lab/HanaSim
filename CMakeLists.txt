cmake_minimum_required(VERSION 3.22.1)
project(HanaSim CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

find_package(TBB REQUIRED)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB Sources src/*.cpp)
file(GLOB Bots bots/*.cpp)
file(GLOB Agents agents/*.cpp)
set (SOURCES ${Sources} ${Bots} ${Agents})

pybind11_add_module(hana_sim ${SOURCES})

target_link_libraries(hana_sim
    PRIVATE
        pybind11::module
        OpenMP::OpenMP_CXX
        TBB::tbb
)

execute_process(
  COMMAND "${Python3_EXECUTABLE}" -c "import site; print(site.getsitepackages()[0])"
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
install(TARGETS hana_sim
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
        RUNTIME DESTINATION ${PYTHON_SITE_PACKAGES})
