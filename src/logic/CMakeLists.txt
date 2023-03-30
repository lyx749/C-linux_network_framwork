include_directories(../include)
include_directories(../logs)
add_library(
    logic
    STATIC
    http_c_logic.cxx
)

target_link_libraries(logic PUBLIC net log)