add_library(
    proc
    STATIC
    http_c_daemon.cxx
    http_c_process_cycle.cxx   
)


target_link_libraries(proc PRIVATE log func)
target_include_directories(proc PUBLIC ../include)