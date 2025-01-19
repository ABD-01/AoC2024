add_custom_target(copyDay9Script
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/main.py
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/day9.py
)

add_dependencies(day9 copyDay9Script)

target_include_directories(day9
    PUBLIC
    "C:/Users/Muhammed/AppData/Local/Programs/Python/Python310/include/"
)

target_link_directories(day9
    PRIVATE
    "C:/Users/Muhammed/AppData/Local/Programs/Python/Python310/libs/"
)

target_link_libraries(day9
    PRIVATE
    "python310.lib"
)

target_compile_options(day9 PRIVATE -O3)
