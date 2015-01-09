cmake_minimum_required(VERSION 3.1)

add_subdirectory(xmas)
add_subdirectory(xmd)
add_subdirectory(test)
add_subdirectory(testmain)
add_subdirectory(xmasmain)


include(CTest)

add_test(checker-test ${xMAS-checker_BINARY_DIR}/xMAS-checker-test)
add_test(xmas-test ${testmain_BINARY_DIR}/testmain) 