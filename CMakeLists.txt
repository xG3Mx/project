cmake_minimum_required(VERSION 2.8)	 

project(hrh)		

set(SOURCE_EXE side.cpp)	

set(SOURCE_LIB main.cpp)		

add_library(main STATIC ${SOURCE_LIB})

add_executable(side ${SOURCE_EXE})	

target_link_libraries(side main)	