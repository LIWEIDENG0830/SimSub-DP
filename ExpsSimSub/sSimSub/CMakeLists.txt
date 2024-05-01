cmake_minimum_required(VERSION 3.18)
project(trajcSimilar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "")
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")


add_executable(application main.cpp constant.h distance.cpp distance.h  
                utils.cpp utils.h
                constriantLength.cpp TrajSim.h TrajSim.cpp)