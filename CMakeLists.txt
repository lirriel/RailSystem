cmake_minimum_required(VERSION 3.6)
project(prj)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp City.h RailSystem.cpp RailSystem.h Service.h)
add_executable(prj ${SOURCE_FILES})