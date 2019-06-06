cmake_minimum_required(VERSION 3.14)
project(p4 C)

set(CMAKE_C_STANDARD 99)

add_executable(p4 pingpong-scheduler.c ppos.c queue.c)