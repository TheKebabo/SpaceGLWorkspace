#pragma once
#include "engine_export.h"
#include <string>

// Forward declaration so we don't have to include GLFW in the header
struct GLFWwindow;

namespace Engine {

    class ENGINE_API Window {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        // Prevent copying (you don't want two objects managing one GPU window)
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        
        bool IsKeyPressed(int key) const;

        bool ShouldClose() const;
        void SwapBuffers();
        void PollEvents();
        void Close();


        
        // Helper to get the underlying pointer if needed
        GLFWwindow* GetNativeWindow() const { return m_Window; }

    private:
        GLFWwindow* m_Window;
        int m_Width, m_Height;
        std::string m_Title;
    };
}