cmake_minimum_required(VERSION 3.2)

project(rikcgi-login)

include_directories("${PROJECT_BINARY_DIR}")
# include_directories("${PROJECT_SOURCE_DIR}/include")

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

install (TARGETS ${PROJECT_NAME}
	RUNTIME
	DESTINATION bin)

# install (FILES ${PROJECT_BINARY_DIR}/dscan.conf DESTINATION /etc/dscan COMPONENT config)

