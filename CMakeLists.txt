cmake_minimum_required(VERSION 3.27.0)
project(
    AoC2024 
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Advent of Code 2024"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

MESSAGE(STATUS "WELCOME TO ADVENT OF CODE 2024")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

function(add_aoc_day day dir)
    set(WORKING_DIR ${CMAKE_SOURCE_DIR}/${dir})
    set(target day${day})

    IF (EXISTS ${WORKING_DIR})

        MESSAGE(STATUS "Processing Day ${day}: ${WORKING_DIR}")
        IF (EXISTS ${WORKING_DIR}/input.txt)
            add_executable(${target} ${WORKING_DIR}/main.cpp)
            add_custom_command(
                TARGET ${target} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy 
                ${WORKING_DIR}/input.txt 
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/input${day}.txt
                COMMENT "Copying input.txt to input${day}.txt"
            )
            add_custom_target(run${day}
                COMMAND ${target} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/input${day}.txt
                DEPENDS ${target}
            )

            add_dependencies(run run${day})

        ELSE()
            MESSAGE(WARNING "Day ${day} does not have an input file")
        ENDIF()

    ELSE()
        MESSAGE(WARNING "Day ${day} does not exist")
    ENDIF()
endfunction()

add_custom_target(run)

add_aoc_day(1 Day01_Historian_Hysteria)
add_aoc_day(2 Day02_Red-Nosed_Reports)
add_aoc_day(3 Day03_Mull_It_Over)
add_aoc_day(4 Day04_Ceres_Search)

