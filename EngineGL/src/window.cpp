#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "EngineCore/window.hpp"

static int s_GLFWWindowCount = 0;

namespace Engine
{
    int Window::s_GLFWWindowCount = 0;
    Window::Window(int width, int height, const std::string& title)
    {
        if (s_GLFWWindowCount == 0)
        {
            // Set global error callback
            glfwSetErrorCallback([](int error, const char* description)
            {
                fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
            });

            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialise GLFW");
            }
        }

        // Set OpenGL 4.6 Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_width = width;
        m_height = height;
        m_title = title;
    
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        
        if (!m_window)
        {
            if (s_GLFWWindowCount == 0) glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        
        glfwSetWindowUserPointer(m_window, this); // For callbacks

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1); // Enable VSync

        // Set glfw mouse configs
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_window, m_mouseMoveCallback);
        glfwSetCursorPos(m_window, (double)m_width / 2.0, (double)m_height / 2.0);
        m_prevMouseX = m_width / 2.0;
        m_prevMouseY = m_height / 2.0;
        m_mouseX = m_prevMouseX;
        m_mouseY = m_prevMouseY;

        // Initialize GLAD (now that we have a current context)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        // Set the viewport
        glViewport(0, 0, m_width, m_height);

        s_GLFWWindowCount++;
    }

    Window::~Window()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
        }
        s_GLFWWindowCount--;

        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate(); // Only shut down when the LAST window is closed
        }
    }

    bool Window::isKeyPressed(int key) const
    {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    void Window::clearSetColour(float r, float g, float b)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, 1.0f);
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    double Window::getTime()
    {
        return glfwGetTime();
    }

    void Window::m_mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY)
    {
        Window* instance = (Window*)glfwGetWindowUserPointer(window);

        // instance->setMousePrev(instance->getMouseX(), instance->getMouseY()); 
        instance->setMouse(mouseX, instance->height() - mouseY); // Y is switched

        // std::cout << "Delta: " << instance->getMouseX() << ", " << instance->getMouseY() << std::endl;
    }
}