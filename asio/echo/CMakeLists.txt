file(GLOB SRCS *.cpp *.h)

add_executable(asio_echo ${SRCS})
target_link_libraries(asio_echo pthread)
