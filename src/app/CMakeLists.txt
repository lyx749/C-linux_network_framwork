include_directories(../include) 

add_library(
    func
    STATIC
    http_setproctitle.cxx
)

target_include_directories(func PUBLIC ../include) 