# SpaceGL/CMakeLists.txt
add_executable(SpaceGL src/winmain.cpp)

# Link to the Engine
target_link_libraries(SpaceGL PRIVATE EngineCore)

# Point to the Engine's headers
target_include_directories(SpaceGL PRIVATE "../EngineGL/include")