cmake_minimum_required(VERSION 3.5)

# Define compile flags 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

# pre path
set(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(JSON_SRC ${CURRENT_DIR}/src)
set(JSON_INCLUDE ${CURRENT_DIR}/include)

# src path
aux_source_directory(${JSON_SRC}/lib_json  LIB_JSON_SRC)
aux_source_directory(${JSON_SRC}/test_lib_json  TEST_LIB_JSON_SRC)
aux_source_directory(${JSON_SRC}/jsontestrunner  JSON_TEST_RUNNER_SRC)
        
        
############################################################
# Create json library
############################################################

#Generate the shared library from the library sources
add_library(libjson SHARED 
    ${LIB_JSON_SRC}
)

target_include_directories(libjson
    PUBLIC 
        ${JSON_INCLUDE}
)

############################################################
# Create demo binary
############################################################

# build json_test demo
add_executable(json_test 
    ${TEST_LIB_JSON_SRC}
)

target_link_libraries(json_test 
    PRIVATE
        libjson
)

# build jsontestrunner demo
add_executable(jsontestrunner 
    ${JSON_TEST_RUNNER_SRC}
)

target_link_libraries(jsontestrunner 
    PRIVATE
        libjson
)


