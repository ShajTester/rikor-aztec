cmake_minimum_required(VERSION 3.2)

project(rikvncs)

include_directories("${PROJECT_BINARY_DIR}")

find_package(Threads)

add_executable(${PROJECT_NAME} example.c)

set_target_properties(${PROJECT_NAME} PROPERTIES
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(${PROJECT_NAME}
	${CMAKE_THREAD_LIBS_INIT}
	vncserver
)


install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

