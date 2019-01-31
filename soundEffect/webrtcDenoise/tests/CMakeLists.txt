cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 14)

include_directories(../src)
add_executable(webrtcDenoise webrtcDenoise.cpp)

if(WIN32)
	target_link_libraries(webrtcDenoise ns)
else(WIN32)
	target_link_libraries(webrtcDenoise ns pthread)
endif(WIN32)


