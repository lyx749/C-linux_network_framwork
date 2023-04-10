cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME httpServer)
set(PROJECT_VERSION "1.0.0.10" CACHE STRING "默认版本号" FORCE)
project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX C
)
include(configure.cmake)
SetConfigure()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/_staticLib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/_bin)
add_executable(httpServer main.cpp)
add_executable(client httpServerClient.cpp)

#判断内存是否完全释放AddressSanitizer
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize=address -Wall -g" 
    )
    target_link_options(
        httpServer 
        BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
    )

elseif(CMAKE_BUILD_TYPE MATCHES "Releases")
    set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -g" 
    )
endif()
target_link_libraries(httpServer PUBLIC net threadPool logic log)
target_link_libraries(client PUBLIC crc32 pthread)
include(CTest)
enable_testing()
add_subdirectory(src)

include(CPack)
