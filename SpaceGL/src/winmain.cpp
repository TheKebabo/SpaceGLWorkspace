#include <iostream> 
#include <EngineCore/window.hpp>
#include <EngineCore/key_codes.h>
#include "simulator.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>

bool firstMouse = true; // To stop initial camera jump
void checkKeyPresses(Engine::Window& window, SpaceGL::Camera& cam, float dt);

int main(int argc, char* argv[])
{
    Engine::Window window(1280, 720, "SpaceGL");
    
    if (firstMouse)
    {
        window.setMousePrev(window.getMouseX(), window.getMouseY()); // Delta is 0
        firstMouse = false;
    }

    double mouseDX = window.getMouseX() - window.getPrevMouseX();
    double mouseDY = window.getMouseY() - window.getPrevMouseY();
    SpaceGL::Simulator sim(window.width(), window.height(), mouseDX, mouseDY, 1);

    double prevT = window.getTime();
    while (!window.shouldClose())
    {
        // std::cout << "loop" << std::endl;
        double curT = window.getTime();
        double dt = curT - prevT;
        checkKeyPresses(window, sim.camera(), dt);

        window.clearSetColour(0.15f, 0.05f, 0.05f);

            if (dt > 0.0)
            {
                mouseDX = window.getMouseX() - window.getPrevMouseX();
                mouseDY = window.getMouseY() - window.getPrevMouseY();

                sim.update(mouseDX, mouseDY, (float)dt);

                window.setMousePrev(window.getMouseX(), window.getMouseY());
                prevT = curT;
            }

        window.swapBuffers();
        window.pollEvents();

    }

    return 0;
}

void checkKeyPresses(Engine::Window& window, SpaceGL::Camera& cam, float dt)
{
    if (window.isKeyPressed(ENGINE_KEY_ESCAPE))
    {
        window.close();
    }
    if (window.isKeyPressed(GLFW_KEY_W))
    {
        cam.move(SpaceGL::MouseDir::FORWARD, dt);
    }
    if (window.isKeyPressed(GLFW_KEY_S))
    {
        cam.move(SpaceGL::MouseDir::BACK, dt);
    }
    if (window.isKeyPressed(GLFW_KEY_A))
    {
        cam.move(SpaceGL::MouseDir::LEFT, dt);
    }
    if (window.isKeyPressed(GLFW_KEY_D))
    {
        cam.move(SpaceGL::MouseDir::RIGHT, dt);
    }
    if (window.isKeyPressed(GLFW_KEY_SPACE))
    {
        cam.move(SpaceGL::MouseDir::UP, dt);
    }
    if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        cam.move(SpaceGL::MouseDir::DOWN, dt);
    }
}