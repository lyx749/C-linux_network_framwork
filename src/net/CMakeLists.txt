include_directories(../include)

add_library(
    net
    STATIC
    http_c_socket.cxx
    http_connect_socket.cxx
)

target_link_libraries(net PRIVATE memory)
target_include_directories(net PUBLIC ../include)