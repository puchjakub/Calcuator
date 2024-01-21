cmake_minimum_required(VERSION 3.14)
project(CalculatorVisual)

# Define the main executable
add_executable(CalculatorVisual WIN32 Calculator.cpp CalculatorVisual.cpp "CalculatorMode.h") # Added WIN32 here

# If you have any additional include directories, list them here
target_include_directories(CalculatorVisual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing with Google Test
enable_testing()

add_executable(CalculatorTests CalculatorTests.cpp Calculator.cpp "CalculatorMode.h")
target_link_libraries(CalculatorTests gtest_main)


include(GoogleTest)
gtest_discover_tests(CalculatorTests)