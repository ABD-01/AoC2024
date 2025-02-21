add_executable(day11c ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_dependencies(day11 day11c)

target_compile_options(day11c
    PRIVATE
        -O3
        -Wall
        -Wextra
    )
