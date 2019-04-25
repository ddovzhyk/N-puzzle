cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 14)

project(npuzzle)
add_executable(${PROJECT_NAME}  "main.cpp"
                                "Parser.h" "Parser.cpp"
                                "Node.h" "Node.cpp"
                                "Algorithm.h" "Algorithm.cpp"
)
