file(GLOB SRCS *.cc
		  ${PROJECT_SOURCE_DIR}/3rdparty/gtest/src/gtest-all.cc
		  ${PROJECT_SOURCE_DIR}/3rdparty/gtest/src/gtest_main.cc)

add_executable(cpp11_research ${SRCS})
target_link_libraries(cpp11_research pthread)

include (CTest)
add_test(NAME cpp11_research COMMAND cpp11_research)
