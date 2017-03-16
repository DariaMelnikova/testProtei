cmake_minimum_required (VERSION 2.6)
set (CMAKE_INCLUDE_PATH include) 
set (PROJECT server)
file (GLOB COMMON_SRC *.cpp)
set (SOURCE ${COMMON_SRC})

add_definitions(-Wall -Werror -std=c++11)

add_executable(${PROJECT} ${SOURCE}) 
