cmake_minimum_required(VERSION 3.20)
project(SpaceGL_Project LANGUAGES C CXX)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- THE UNIFIED OUTPUT DIRECTORIES ---
# Send all .exe and .dll files to the "bin" folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Send all .lib (import libraries) to the "lib" folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# --- Sub-Projects ---
# Build the Engine first
add_subdirectory(EngineGL)

# Build the Game second
add_subdirectory(SpaceGL)