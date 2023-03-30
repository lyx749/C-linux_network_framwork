include_directories(../include)
include_directories(spdlog/include)
add_library(
    log
    STATIC
    http_log.cxx
)

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.9.2
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog # 指定库下载地址
)

FetchContent_MakeAvailable(spdlog)
target_link_libraries(log PUBLIC spdlog::spdlog)
target_include_directories(log PUBLIC .)