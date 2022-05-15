cmake_minimum_required(VERSION 3.10)

project(MatrixResolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#========================
#Sources
#========================
file(GLOB src "src/*.cpp")

add_executable(${PROJECT_NAME} ${src})
