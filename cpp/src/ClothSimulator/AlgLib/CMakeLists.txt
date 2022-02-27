message("Found alglib")

add_library(AlgLib)

file(GLOB AlgLibSources src/*.cpp src/*.h)
message("AlgLibSources: " ${AlgLibSources})
target_sources(AlgLib PRIVATE ${AlgLibSources})
file(GLOB AlgLibIncludes int/*.h )
message("AlgLibIncludes: " ${AlgLibIncludes})
target_sources(AlgLib PUBLIC ${AlgLibIncludes})

target_include_directories(AlgLib PUBLIC src)
target_include_directories(AlgLib PUBLIC inc)
target_include_directories(AlgLib PUBLIC inc/alglib)
