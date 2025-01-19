add_custom_target(copyDay9Script
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/main.py
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/day9.py
)
add_dependencies(day9 copyDay9Script)

find_package(Python3 COMPONENTS Interpreter Development)

if(NOT Python3_FOUND)
    MESSAGE(WARNING "Did not find Python3 on your system.
    See https://cmake.org/cmake/help/v3.29/module/FindPython3.html for more information.")

    set(Python3_ROOT_DIR "C:/Users/Muhammed/AppData/Local/Programs/Python/Python310")
    set(Python3_INCLUDE_DIRS "${Python3_ROOT_DIR}/include/")
    set(Python3_LIBRARIES "${Python3_ROOT_DIR}/libs/python310.lib")
endif()

target_include_directories(day9
    PRIVATE
    ${Python3_INCLUDE_DIRS}
)

# target_link_directories(day9
#     PRIVATE
#     "C:/Users/Muhammed/AppData/Local/Programs/Python/Python310/libs/"
# )


target_link_libraries(day9
    PRIVATE
    ${Python3_LIBRARIES}
)

if(NOT WIN32)
    # Link pthread, dl, and util on non-Windows platforms
    target_link_libraries(day9 PRIVATE pthread dl util)
endif()