include_directories(../include)
add_library(
    log
    STATIC
    http_log.cxx   
)

target_link_libraries(log PRIVATE memory func)