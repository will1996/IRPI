cmake_minimum_required(VERSION 3.2)
project(IRPI)
set(CMAKE_CXX_STANDARD 20)
enable_testing()
add_subdirectory(cpp/src)



add_subdirectory(pybind11)

find_package(Python COMPONENTS Development REQUIRED)


#collect python wrapper lib files
file(GLOB pylib_files python/*.cpp python/*.hpp)

pybind11_add_module(IRPIPy SHARED ${pylib_files})


target_link_libraries(IRPIPy PUBLIC ClothSimulator )
target_link_libraries(IRPIPy PUBLIC Python::Python)
target_link_libraries(IRPIPy PUBLIC Python::Module)

#add_subdirectory(cpp/test)
