include_directories(../include)
add_library(
    memory
    STATIC
    http_c_memory.cxx
)
# set_target_properties(
#     memory
#     PROPERTIES
#     CXX_STANDARD 17  
# )
target_include_directories(memory PUBLIC ../include)