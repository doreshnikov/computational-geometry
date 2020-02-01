cmake_minimum_required(VERSION 3.15)
project(computational_geometry)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

function(TASK letter)
    file(GLOB name src/${letter}.*.cpp)
    get_filename_component(sname ${name} NAME)
    message("Task '${letter}': ${sname}")

    set(target cg_${letter})
    message("-- Executable ${target}")
    add_executable(${target} ${name})
    target_compile_definitions(${target} PUBLIC LOCAL)
    target_link_libraries(${target} PUBLIC gmp)

    message("..done")
endfunction()

function(TASKS list)
    foreach(task ${list})
        TASK(${task})
    endforeach()
endfunction()

TASKS("A;B;C;E;G;H;I;J;K;L;M;N")