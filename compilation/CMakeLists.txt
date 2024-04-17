cmake_minimum_required(VERSION 3.0) 		# setting this is required
project(hello)		            		# this sets the project name

file(GLOB_RECURSE sources      ./main.c utils.c)
#file(GLOB_RECURSE sources      ./*.c) 		# won't work if source files not placed in dedicated dir like src...

add_executable(hello.out ${sources})

add_library(utils_lib SHARED utils.c)
target_link_libraries(hello.out PUBLIC utils_lib)

# target_include_directories(hello PUBLIC include)
