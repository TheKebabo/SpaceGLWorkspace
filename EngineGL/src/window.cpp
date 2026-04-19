#include <glad.h>
#include <GLFW/glfw3.h>
#include "EngineCore/window.h"

static int s_GLFWWindowCount = 0;


namespace Engine {

    Window::Window(int width, int height, const std::string& title) {
        if (s_GLFWWindowCount == 0) {
            glfwInit();
            // Set global error callback
            glfwSetErrorCallback([](int error, const char* description) {
                fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
            });
        }

        if (!glfwInit()) {
            // Handle error: Logging or Throwing exception
            throw; // TODO: handle better
        }

        glfwSetWindowUserPointer(m_Window, this);

        // 4. Now it is safe to set your callbacks, because if the user 
        // presses a key, the callback can successfully fetch 'this'
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Window* instance = (Window*)glfwGetWindowUserPointer(window);
            
            // Example: Let the user close the window by hitting ESCAPE
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
        });

        // Set OpenGL 4.6 Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        
        if (!m_Window) {
            glfwTerminate();
            // Handle error
            throw; // TODO: handle better 
        }

        glfwMakeContextCurrent(m_Window);

        // Initialize GLAD (now that we have a current context)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            // Handle error: Failed to initialize GLAD
            throw; // TODO: handle better 
        }

        // Set the viewport
        glViewport(0, 0, m_Width, m_Height);

        s_GLFWWindowCount++;
    }

    Window::~Window() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
        }
        s_GLFWWindowCount--;

        if (s_GLFWWindowCount == 0) {
            glfwTerminate(); // Only shut down when the LAST window is closed
        }
    }

    bool Window::IsKeyPressed(int key) const {
        // Returns true if the key is currently pressed
        int state = glfwGetKey(m_Window, key);
        return state == GLFW_PRESS;
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(m_Window);
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void Window::Close() {
        glfwSetWindowShouldClose(m_Window, true);
    }
}