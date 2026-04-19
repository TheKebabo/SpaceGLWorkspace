#include <EngineCore/window.h>
#include <EngineCore/key_codes.h>
#include <iostream>

int main() {
    // 1. Create the window using our Engine
    Engine::Window window(1280, 720, "SpaceGL");

    // 2. The Loop
    while (!window.ShouldClose()) {
        if (window.IsKeyPressed(ENGINE_KEY_ESCAPE)) {
            window.Close();
        }

        window.SwapBuffers();
        window.PollEvents();
    }

    return 0;
}