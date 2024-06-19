# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(s21_brick_game VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -g")

# Check if coverage is enabled
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

# Find and include ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src/brick_game/base
    ${PROJECT_SOURCE_DIR}/src/brick_game/snake
    ${PROJECT_SOURCE_DIR}/src/brick_game/tetris
    ${PROJECT_SOURCE_DIR}/src/brick_game/controller
    ${PROJECT_SOURCE_DIR}/src/gui/console/base
    ${PROJECT_SOURCE_DIR}/src/gui/console/snake
    ${PROJECT_SOURCE_DIR}/src/gui/console/tetris
    ${PROJECT_SOURCE_DIR}/src/tests
)

# Add Google Test
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/googletest")
    message(STATUS "Google Test not found, cloning...")
    execute_process(COMMAND git clone https://github.com/google/googletest.git
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
endif()
add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include)

# Add the main library
file(GLOB BASE_SOURCES "src/brick_game/base/*.cpp")
file(GLOB SNAKE_SOURCES "src/brick_game/snake/*.cpp")
file(GLOB TETRIS_SOURCES "src/brick_game/tetris/*.cpp")
file(GLOB CONTROLLER_SOURCES "src/brick_game/controller/*.cpp")
file(GLOB CONSOLE_BASE_SOURCES "src/gui/console/base/*.cpp")
file(GLOB CONSOLE_SNAKE_SOURCES "src/gui/console/snake/*.cpp")
file(GLOB CONSOLE_TETRIS_SOURCES "src/gui/console/tetris/*.cpp")

add_library(s21_brick_game STATIC ${BASE_SOURCES} ${SNAKE_SOURCES} ${TETRIS_SOURCES} ${CONTROLLER_SOURCES})

# Add executables
file(GLOB TEST_SOURCES "tests/*.cpp")

# Include the console main.cpp
set(GUI_CONSOLE_SOURCES
    ${CONSOLE_BASE_SOURCES}
    ${CONSOLE_SNAKE_SOURCES}
    ${CONSOLE_TETRIS_SOURCES}
    ${CONSOLE_SOURCES}
    src/gui/console/ConsoleView.cpp
    src/gui/console/main.cpp
)

add_executable(brick_game_console ${GUI_CONSOLE_SOURCES})
add_executable(run_tests ${TEST_SOURCES})

# Link libraries
target_link_libraries(brick_game_console s21_brick_game ${CURSES_LIBRARIES})
target_link_libraries(run_tests s21_brick_game gtest gtest_main pthread)

# Add subdirectory for the desktop version
add_subdirectory(src/gui/desktop)

# Enable testing
enable_testing()
add_test(NAME run_tests COMMAND run_tests)

# Custom target to run tests and generate code coverage report
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)
if(LCOV_PATH AND GENHTML_PATH)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${LCOV_PATH} --capture --directory . --output-file coverage.info
        COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/external/*' --output-file coverage.info
        COMMAND ${GENHTML_PATH} coverage.info --output-directory coverage_report
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests and generating code coverage report"
    )
else()
    message(STATUS "lcov or genhtml not found, coverage target will not be available")
endif()
