add_custom_target(copyDay7Script
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/main.py
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/day7.py
)

add_dependencies(day7 copyDay7Script)

target_sources(day6
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/util.h
)
target_compile_options(day7
    PRIVATE
        -O3
        -Wall
        -Wextra
    )
