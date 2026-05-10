include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        "1.0.1"
)
FetchContent_MakeAvailable(glm)

# --- Sources ---
set(SPACEGL_SOURCES
    "src/winmain.cpp"
    "src/simulator.cpp"
    "src/buffers_handler.cpp"
    "src/renderer.cpp"
    "src/camera.cpp"
    "src/resource_handler.cpp"
)

add_executable(SpaceGL ${SPACEGL_SOURCES})

target_compile_definitions(SpaceGL PRIVATE 
    SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)
target_compile_definitions(SpaceGL PRIVATE 
    ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)

# Point to the headers
target_include_directories(SpaceGL PRIVATE
    "include"
    "../EngineGL/include"
    "${glm_SOURCE_DIR}")

# Link to the Engine
target_link_libraries(SpaceGL PRIVATE EngineCore)

if(MINGW)
    # This forces the linker to use the console subsystem and find 'main'
    target_link_options(SpaceGL PRIVATE "-Wl,--subsystem,console")
endif()