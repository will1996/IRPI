macro(addTest testName sourceFile LibList)
 add_executable(${testName} ${sourceFile})
 target_link_libraries(${testName} PUBLIC "${LibList}")
 add_test(${testName} ${CMAKE_BINARY_DIR}/${testName} )
endmacro()



addTest(Passes always_passes.cpp ClothSimulator)
addTest(ClothDataGenTest ClothDataGen.cpp ClothSimulator)
addTest(taucsLinksTest taucsLinksTest.cpp Taucs)



