include_directories (${PROJECT_SOURCE_DIR}
                     ${Boost_INCLUDE_DIR}
					 ${PROJECT_SOURCE_DIR}/3rdparty/folly-2017.04.03.00)

message (STATUS "Boost_INCLUDE_DIR=${Boost_INCLUDE_DIR}")
add_subdirectory (mpmc_queue)
