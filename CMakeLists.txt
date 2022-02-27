cmake_minimum_required(VERSION 3.2)
project(IRPI)
set(CMAKE_CXX_STANDARD 17)
enable_testing()
add_subdirectory(cpp/src)
add_subdirectory(cpp/test)



find_package(pybind11 REQUIRED)

#collect python wrapper lib files
file(GLOB pylib_files python/*.cpp python/*.hpp)
pybind11_add_module(IRPIPy ${pylib_files})

target_link_libraries(IRPIPy PUBLIC ClothSimulator)
