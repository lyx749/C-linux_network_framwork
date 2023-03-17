cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME httpServer)
set(PROJECT_VERSION "1.0.0.10" CACHE STRING "默认版本号" FORCE)
project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX C
)

include(CTest)
enable_testing()

add_executable(httpSever main.cpp)
include(CPack)
