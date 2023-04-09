include_directories(../include)
add_library(
    httpParser
    STATIC
    http_parser.cxx   
)

target_include_directories(httpParser PUBLIC .)

