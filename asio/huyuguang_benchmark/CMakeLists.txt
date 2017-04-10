file(GLOB SRCS *.cpp *.h)

add_executable(asio_huyuguang_benchmark ${SRCS})
target_link_libraries(asio_huyuguang_benchmark pthread)

