cmake_minimum_required(VERSION 3.28)
project(HelloCV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(HelloCV main.cpp)

set_target_properties(HelloCV PROPERTIES OUTPUT_NAME "CryptoTool")