target_sources(day6
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/guard.cpp
        ${CMAKE_CURRENT_LIST_DIR}/point.cpp
)

target_include_directories(day6
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_options(day6
    PRIVATE
        -O3
        -Wall
        -Wextra
    )
