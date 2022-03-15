message ("Found Lib Taucs, Executing Special build instructions")


#gfortran is really hard to find on macos, so, we'll locate the file searching a few different spots, 
#and then pass that to make 
#set(GFortranSearchPaths "/usr/local/Cellar/gcc/11.2.0_3/lib/gcc/11/;/usr/lib/")
#
#find_file(StaticGFortranFullPath "libgfortran.a" PATHS ${GFortranSearchPaths})
#find_file(SharedGFortranFullPath "libgfortran.so" PATHS ${GFortranSearchPaths})
#
#if(${StaticGFortranFullPath} STREQUAL "StaticGFortranFullPath-NOTFOUND" AND ${SharedGFortranFullPath} STREQUAL "SharedGFortranFullPath-NOTFOUND")
#    
#    message(FATAL_ERROR "Couldn't find GFORTRAN, Serached: " ${GFortranSearchPaths} "Try installing gfortran, and putting its path in the FortranSearchPaths variable in CMAKE CACHE")
#    else()
#    message("Found GFortan at: " ${GFortranFullPath})
#endif()
#
#get_filename_component(GFortranPath ${GFortranFullPath} DIRECTORY)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/FindGFortranLibs.cmake)
if(${GFORTRAN_LIBRARIES_DIR} STREQUAL "")
    message(FATAL_ERROR "COULDN'T FIND GFORTRAN")
    else()
    list(GET ${GFORTRAN_LIBRARIES_DIR} 0 GFortranPath)
    message("Found gfortranlibs at: " ${GFortranPath})
    endif()

include(ExternalProject)

if(GFortranPath STREQUAL "")
    message(FATAL_ERROR "Couldn't find GFORTRAN")
    else()
    endif()

ExternalProject_Add(taucs 
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/taucs/src
    CONFIGURE_COMMAND cd ${CMAKE_CURRENT_LIST_DIR}/taucs && ./configure module=\!METIS
    BUILD_COMMAND  cd ${CMAKE_CURRENT_LIST_DIR}/taucs && make GFORTRANPATH=${GFortranPath}
    INSTALL_COMMAND ""
)


add_library(Taucs INTERFACE)
add_dependencies(Taucs taucs)

target_link_libraries(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/lib/linux/libtaucs.a)
target_include_directories(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/src)
target_include_directories(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/build/darwin)
target_include_directories(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/build/linux)
