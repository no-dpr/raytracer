cmake_minimum_required(VERSION 3.20.0)
project(Raytracer   VERSION 0.1.0 
                    LANGUAGES CXX)

include_directories(src)

set( SOURCE 
    src/main.cpp
)
set(CMAKE_CXX_STANDARD 11)

add_executable(Raytracer ${SOURCE})

target_include_directories(Raytracer PUBLIC include)