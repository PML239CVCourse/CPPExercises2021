project(lesson01)

set(SOURCES
        src/some_math.cpp
        src/some_math.h
        src/simple_sum.cpp
        src/simple_sum.h)

add_executable(hello src/main.cpp ${SOURCES} src/fib.h src/simple_fib.cpp)

add_executable(lesson01_tests tests/lesson01_tests.cpp ${SOURCES})
target_link_libraries(lesson01_tests gtest_main gtest)
