include_directories(../include)

add_library(
    net
    STATIC
    http_c_socket.cxx
    http_c_socket_connect.cxx
    http_c_socket_accept.cxx
    http_c_socket_request.cxx
)

target_link_libraries(net PRIVATE memory)
target_include_directories(net PUBLIC ../include)