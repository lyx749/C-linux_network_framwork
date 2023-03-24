include_directories(../include)

add_library(
    logic
    STATIC
    http_c_logic.cxx
)

target_link_libraries(logic PUBLIC net)