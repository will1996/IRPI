message("ClothSimulator Lib found")

#--------------Dependencies------------------------------
message("Adding dependencies of ClothSimulator: ")
add_subdirectory("Taucs")
add_subdirectory("AlgLib")
#------------------------------

#The cloth simulator explicitly depends on:
# Torch - pytorch
# Python  - include dirs only
# OMP - TODO rip this pooopoo out



#----------------------------#------------------------------
#Find Torch 
#----------------------------#------------------------------
#Torch uses python to store its cmake module path, invoke python and snag the path in TorchModulePath
execute_process(COMMAND "python3" "-cimport torch;print(torch.utils.cmake_prefix_path)" OUTPUT_VARIABLE TorchModulePath)
#this method of gettings things form python leaves a newline that causes problems, so remove the whitespace
string(STRIP ${TorchModulePath} TorchModulePath)
message("Got torch module path from python: " ${TorchModulePath} )
message("Overriding Cuda search path to environment variable: " $ENV{CUDA_HOME})
set(CUDNN_LIBRARY_PATH /opt/common/cudnn/cudnn-10.2-8.2.1.32/lib64)
set(CUDNN_INCLUDE_PATH /opt/common/cudnn/cudnn-10.2-8.2.1.32/include)
set(CUDA_TOOLKIT_ROOT_DIR $ENV{CUDA_HOME})
find_package(Torch REQUIRED PATHS ${TorchModulePath})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
if(NOT ${TORCH_FOUND})
    message(FATAL_ERROR "LIBTORCH NOT FOUND!  Make sure you install it with: pip3 install torch")
    else()
    message(" Found Torch at: " "${TORCH_LIBRARIES}")
    message("Torch compile flags: ${TORCH_CXX_FLAGS}")
endif()
#----------------------------#------------------------------
#Find python include directories
#----------------------------#------------------------------
execute_process(COMMAND "python3" "-cimport sysconfig;print(sysconfig.get_paths()['include'])" OUTPUT_VARIABLE PythonIncludeDir)

#Make provides horrific errors when you have whitespace in things (which cmake could fix but chooses not to, so here we go)
string(STRIP ${PythonIncludeDir} PythonIncludeDir)

message("Python include dirs " ${PythonIncludeDir})
#----------------------------#------------------------------
#Find OpenMP 
#----------------------------#------------------------------
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message ("Found OpenMP")
else()
    message(FATAL_ERROR "Couldn't find OpenMP! Make sure it is installed (on a mac: brew install libomp) ")
endif()
#----------------------------#------------------------------
#build ClothSimulator library
#----------------------------#------------------------------
add_library(ClothSimulator SHARED)

file(GLOB source_files src/*.cpp src/*.hpp)

message("Source files for ClothSimulator Lib: " ${source_files})

file(GLOB include_files inc/*.hpp)

message("Include files for ClothSimulator Lib: " ${include_files})
#----------------------------#------------------------------
# Link everything up
#----------------------------#------------------------------
target_compile_options(ClothSimulator PUBLIC ${TORCH_CXX_FLAGS})
target_link_libraries(ClothSimulator PRIVATE ${TORCH_LIBRARIES})
target_include_directories(ClothSimulator SYSTEM PRIVATE BEFORE ${PythonIncludeDir})
target_sources(ClothSimulator PRIVATE ${source_files} PUBLIC ${include_files})

target_link_libraries(ClothSimulator PRIVATE AlgLib)
target_link_libraries(ClothSimulator PRIVATE Taucs)
target_link_libraries(ClothSimulator PRIVATE OpenMP::OpenMP_CXX)

target_include_directories(ClothSimulator PRIVATE src)
target_include_directories(ClothSimulator PUBLIC inc)
