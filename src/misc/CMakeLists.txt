include_directories(../include)
add_library(
    memory
    STATIC
    http_c_memory.cxx
)

add_library(
    crc32
    STATIC
    http_c_crc32.cxx  
)

add_library(
    threadPool
    STATIC
    http_c_threadpool.cxx   
)

target_link_libraries(threadPool PUBLIC pthread crc32)
# set_target_properties(
#     memory
#     PROPERTIES
#     CXX_STANDARD 17  
# )
target_include_directories(memory PUBLIC ../include)