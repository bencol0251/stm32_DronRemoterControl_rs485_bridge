cmake_minimum_required(VERSION 3.16)
project(logic_sim C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(logic_sim
    logic_sim.c
    ../../App/protocol.c
    ../../App/key_mapping.c
    ../../App/timer_service.c
)

target_include_directories(logic_sim PRIVATE
    ../../App
)

target_compile_options(logic_sim PRIVATE -Wall -Wextra)
