message ("Found Lib Taucs, Executing Special build instructions")


#gfortran is really hard to find on macos, so, we'll locate the file searching a few different spots, 
#and then pass that to make 
set(GFortranSearchPaths "/usr/local/Cellar/gcc/11.2.0_3/lib/gcc/11/")

find_file(GFortranFullPath "libgfortran.a" PATHS ${GFortranSearchPaths})

if(${GFortranFullPath} STREQUAL "GFortranPath-NOTFOUND")
    
    message(FATAL_ERROR "Couldn't find GFORTRAN, Serached: " ${GFortranSearchPaths} "Try installing gfortran, and putting its path in the FortranSearchPaths variable in CMAKE CACHE")
    else()

    message("Found GFortan at: " ${GFortranFullPath})
endif()

get_filename_component(GFortranPath ${GFortranFullPath} DIRECTORY)

include(ExternalProject)

ExternalProject_Add(taucs 
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/taucs/src
    CONFIGURE_COMMAND cd ${CMAKE_CURRENT_LIST_DIR}/taucs && ./configure module=\!METIS
    BUILD_COMMAND  cd ${CMAKE_CURRENT_LIST_DIR}/taucs && make GFORTRANPATH=${GFortranPath}
    INSTALL_COMMAND ""
)


add_library(Taucs INTERFACE)

target_link_libraries(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/lib/darwin/libtaucs.a)
target_include_directories(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/src)
target_include_directories(Taucs INTERFACE ${CMAKE_CURRENT_LIST_DIR}/taucs/build/darwin)
