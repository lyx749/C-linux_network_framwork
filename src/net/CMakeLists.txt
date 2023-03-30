include_directories(../include)
include_directories(../logs)
add_library(
    net
    STATIC
    http_c_socket.cxx
    http_c_socket_connect.cxx
    http_c_socket_accept.cxx
    http_c_socket_request.cxx
    http_c_socket_time.cxx
)

target_link_libraries(net PUBLIC memory log)
target_include_directories(net PUBLIC ../include)