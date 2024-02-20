cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME project)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 20)

project("${TARGET_NAME}" VERSION 1.0 LANGUAGES CXX)

# Target
file(GLOB TARGET_SRC_FILES CONFIGURE_DEPENDS src/*)
add_library("${TARGET_NAME}" ${TARGET_SRC_FILES})
target_include_directories("${TARGET_NAME}" PUBLIC src PRIVATE lib/csv)

# Movie database file
set(MOVIEDB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/HollywoodMovies.csv")
target_compile_definitions("${TARGET_NAME}" PRIVATE MOVIEDB_PATH="${MOVIEDB_PATH}")

# Main application
add_executable("${TARGET_NAME}_main" main.cpp)
target_link_libraries("${TARGET_NAME}_main" PRIVATE "${TARGET_NAME}")
target_compile_definitions("${TARGET_NAME}_main" PRIVATE MOVIEDB_PATH="${MOVIEDB_PATH}")

# Tests
include(CTest)
file(GLOB TEST_SRC_FILES CONFIGURE_DEPENDS test/*)
add_executable("${TARGET_NAME}_tests" ${TEST_SRC_FILES})
target_include_directories("${TARGET_NAME}_tests" PUBLIC "${TARGET_NAME}")
target_link_libraries("${TARGET_NAME}_tests" "${TARGET_NAME}" Catch2::Catch2WithMain)
target_compile_definitions("${TARGET_NAME}_tests" PRIVATE MOVIEDB_PATH="${MOVIEDB_PATH}")

# Catch2 dependency
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)
include(Catch)

# Enable and find tests
enable_testing()
catch_discover_tests("${TARGET_NAME}_tests")
