project(lesson02vector)

set(SOURCES
        src/helper_function.cpp
        src/helper_function.h)

set(LIBRARIES
        libutils)

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
