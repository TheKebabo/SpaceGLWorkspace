# EngineGL/CMakeLists.txt

# --- 1. Fetch GLFW ---
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4 
)

# Turn off GLFW's extra stuff so it builds instantly
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# --- 2. Sources ---
set(ENGINE_SOURCES
    "src/shader_program.cpp"
    "src/vf_shader_program.cpp"
    "src/compute_shader_program.cpp"
    "src/window.cpp"
    "external/glad/src/glad.c"
)

# # --- 3. Build as a DLL ---
# add_library(EngineCore SHARED ${ENGINE_SOURCES})
add_library(EngineCore STATIC ${ENGINE_SOURCES})

# --- 4. Include Paths ---
target_include_directories(EngineCore PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"               # Your Engine headers
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/glad" # GLAD & KHR headers
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/KHR" # GLAD & KHR headers
    "${glfw_SOURCE_DIR}/include"                        # GLFW headers downloaded by FetchContent
)

# --- 5. Target Properties ---
# Force it to use the C++ linker since we are mixing C (GLAD) and C++ (Engine)
set_target_properties(EngineCore PROPERTIES LINKER_LANGUAGE CXX)

# --- 6. Trigger the DLL Export Macro ---
target_compile_definitions(EngineCore PRIVATE BUILDING_MY_ENGINE)

# --- 7. Link Dependencies ---
target_link_libraries(EngineCore PUBLIC opengl32 glfw gdi32)