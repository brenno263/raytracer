cmake_minimum_required(VERSION 3.20)
project(raytracer_cmake)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -lpthread)

add_executable(raytracer_cmake main.cpp triangle.h plane.h material.h collision.h tilecounter.h)
