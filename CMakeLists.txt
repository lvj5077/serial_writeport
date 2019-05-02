cmake_minimum_required(VERSION 2.8.3)
project(faulhaber)

SET(CMAKE_CXX_COMPILER "g++")
SET( CMAKE_BUILD_TYPE Release )
SET( CMAKE_CXX_FLAGS "-O3 -std=c++11" )

add_library( myMotor SHARED src/motor.cpp)
target_link_libraries(myMotor -lboost_unit_test_framework  -lserial -lpthread)

add_executable(motorctl src/test_motor.cpp)
target_link_libraries(motorctl myMotor -lboost_unit_test_framework  -lserial -lpthread)