cmake_minimum_required(VERSION 3.1)

project(libutils)

set(HEADERS
        libutils/fast_random.h
        libutils/rasserts.h
        libutils/timer.h
        )

set(SOURCES
        libutils/rasserts.cpp
        )

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
