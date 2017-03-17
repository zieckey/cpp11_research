cmake_minimum_required (VERSION 2.8)

project (CPP11Research C CXX)

if (UNIX)
    set (CXX_FLAGS
         -g
         -fPIC
         -Wall
         -Wno-unused-parameter
         -Wno-unused-function
         -Wunused-variable
         -Wunused-value
         -Wshadow
         -Wcast-qual
         -Wcast-align
         -Wwrite-strings
         -Wsign-compare
         -Winvalid-pch
         -fms-extensions
         -Wfloat-equal
         -Wextra
         -std=c++11
    )
endif (UNIX)


include_directories (${PROJECT_SOURCE_DIR}
					 ${PROJECT_SOURCE_DIR}/3rdparty
					 ${PROJECT_SOURCE_DIR}/3rdparty/gtest)

string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if (UNIX)
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -D_DEBUG -DGOOGLE_STRIP_LOG=0")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DGOOGLE_STRIP_LOG=1")
endif (UNIX)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set (CMAKE_MACOSX_RPATH 0)

message (STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message (STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message (STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
message (STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message (STATUS "LIBRARY_OUTPUT_PATH " ${LIBRARY_OUTPUT_PATH})

add_subdirectory (src)

include (CPack)
include (CTest)
