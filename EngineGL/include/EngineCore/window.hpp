#pragma once
#include <string>
#include "engine_export.h"

// Forward declaration so we don't have to include GLFW in the header
struct GLFWwindow;

namespace Engine
{

    class ENGINE_API Window
    {
        public:

        Window(int width, int height, const std::string& title);
        ~Window();

        // Prevent copying (you don't want two objects managing one GPU window)
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        
        int width() { return m_width; }
        int height() { return m_height; }

        void setMousePrev(double x, double y) { m_prevMouseX = x; m_prevMouseY = y; }
        void setMouse(double x, double y) { m_mouseX = x; m_mouseY = y; }
        double getPrevMouseX() { return m_prevMouseX; }
        double getPrevMouseY() { return m_prevMouseY; }
        double getMouseX() { return m_mouseX; }
        double getMouseY() { return m_mouseY; }

        bool isKeyPressed(int key) const;
        void clearSetColour(float r, float g, float b);

        bool shouldClose() const;
        void swapBuffers();
        void pollEvents();
        void close();
        double getTime();

        // Helper to get the underlying pointer if needed
        GLFWwindow* getNativeWindow() const { return m_window; }

    private:
        GLFWwindow* m_window;
        int m_width = 800, m_height = 600;
        std::string m_title;
        static int s_GLFWWindowCount;

         // X goes right, Y goes up
        double m_prevMouseX = 0.0; double m_prevMouseY = 0.0;
        double m_mouseX = 0.0; double m_mouseY = 0.0;
        static void m_mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY);
    };
}