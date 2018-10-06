cmake_minimum_required(VERSION 3.12)
project(clion C)

set(CMAKE_C_STANDARD 99)

add_executable(clion main.c getTemp.c initialize.c enums_structs_etc.h function_decs.h makeDecision.c readSetpoints.c cJSON.h cJSON.c)
