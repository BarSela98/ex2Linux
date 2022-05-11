
# Minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.0)

# Name of the project
project(ex2.out)

# add the src library
add_subdirectory(src)



# Add all the source files needed to build the executable
add_executable(${PROJECT_NAME} main.cpp)

find_library(libzip zip)


target_link_libraries(${PROJECT_NAME} PUBLIC "${libzip}")
target_link_libraries(${PROJECT_NAME} PUBLIC src)

target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_SOURCE_DIR}/src"
                        
                          )
