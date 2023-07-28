cmake_minimum_required(VERSION 3.22.0)
project(FileFinder)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#_____Create library

file(GLOB lib_src 
	source/Library/*.cpp 
	source/Library/*.h
)
add_library(${PROJECT_NAME}_lib STATIC ${lib_src})

#_____Create application

file(GLOB src 
	source/Application/*.cpp 
	source/Application/*.h
)
add_executable(${PROJECT_NAME} ${src})

target_include_directories(${PROJECT_NAME} PRIVATE source/Library/*.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
	
	

