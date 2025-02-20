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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-sign-compare -O3")

MESSAGE(STATUS "WELCOME TO ADVENT OF CODE 2024")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

function(add_aoc_day day dir)
    set(WORKING_DIR ${CMAKE_SOURCE_DIR}/${dir})
    set(target day${day})

    IF (EXISTS ${WORKING_DIR})

        MESSAGE(STATUS "Processing Day ${day}: ${WORKING_DIR}")
            add_executable(${target} ${WORKING_DIR}/main.cpp)
            add_custom_target(run${day}
                COMMAND ${target} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/input${day}.txt
                DEPENDS ${target}
            )
            add_dependencies(run run${day})
            
        IF (EXISTS ${WORKING_DIR}/input.txt)
            add_custom_command(
                TARGET ${target} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy 
                ${WORKING_DIR}/input.txt 
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/input${day}.txt
                COMMENT "Copying input.txt to input${day}.txt"
            )
        ELSE()
            MESSAGE(WARNING "Day ${day} does not have an input file")
        ENDIF()
    ELSE()
        MESSAGE(WARNING "Day ${day} does not exist")
    ENDIF()
endfunction()

# Add an option for treating warnings as errors
option(ENABLE_WERROR "Treat warnings as errors" OFF)

if(ENABLE_WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

add_custom_target(run)

add_aoc_day(1 Day01_Historian_Hysteria)
add_aoc_day(2 Day02_Red-Nosed_Reports)
add_aoc_day(3 Day03_Mull_It_Over)
add_aoc_day(4 Day04_Ceres_Search)
add_aoc_day(5 Day05_Print_Queue)
add_aoc_day(6 Day06_Guard_Gallivant)
    add_subdirectory(Day06_Guard_Gallivant)
add_aoc_day(7 Day07_Bridge_Repair)
    add_subdirectory(Day07_Bridge_Repair)
add_aoc_day(8 Day08_Resonant_Collinearity)
add_aoc_day(9 Day09_Disk_Fragmenter)
add_aoc_day(10 Day10_Hoof_It)
add_aoc_day(11 Day11_Plutonian_Pebbles)