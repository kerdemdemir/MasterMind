cmake_minimum_required(VERSION 3.20)
project(MasterMindErdemDemr)

set(CMAKE_CXX_STANDARD 23)

add_executable(MasterMindErdemDemr main.cpp Common.h UnitTests/UnitTests.h CodeMaker.h CodeBreaker.h Game.h Strategy.h)
