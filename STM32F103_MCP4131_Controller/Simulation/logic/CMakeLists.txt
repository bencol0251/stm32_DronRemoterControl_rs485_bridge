cmake_minimum_required(VERSION 3.16)
project(logic_sim C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(logic_sim
    logic_sim.c
    ../../Core/Src/protocol.c
    ../../Core/Src/key_mapping.c
    ../../Core/Src/timer_service.c
)

target_include_directories(logic_sim PRIVATE
    ../../Core/Inc
)

target_compile_options(logic_sim PRIVATE -Wall -Wextra)
