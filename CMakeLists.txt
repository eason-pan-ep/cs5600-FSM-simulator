cmake_minimum_required(VERSION 3.26)
project(cs5600_FSM_simulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs5600_FSM_simulator main.cpp
)
add_executable(TestMemory tests/testMemory.cpp)
add_executable(TestControlView tests/testControlView.cpp)
