#pragma once

#include <glm/glm.hpp>

namespace SpaceGL
{
    // enum MouseDir { FORWARD = GLFW_KEY_W, BACK = GLFW_KEY_S, LEFT = GLFW_KEY_A, RIGHT = GLFW_KEY_D, DOWN = GLFW_KEY_LEFT_SHIFT, UP = GLFW_KEY_SPACE };
    enum MouseDir { FORWARD, BACK, LEFT, RIGHT, DOWN, UP };
    class Camera
    {
    public:
        Camera(glm::vec3& init) { m_pos = init; m_calcFront(); }
        Camera(float x, float y, float z) { m_pos = glm::vec3(x, y, z); m_calcFront(); }

        glm::vec3& getPos() { return m_pos; }
        glm::vec3& getFront() { return m_front; }
        glm::vec3& getUp() { return m_up; }

        void move(MouseDir d, float dt);
        
        float yaw() { return m_yaw; }
        float pitch() { return m_pitch; }
        void updateYaw(float dYaw);
        void updatePitch(float dPitch);

    private:
        void m_calcFront();

        glm::vec3 m_pos {0.0f, 0.0f, 0.0f};
        glm::vec3 m_front {0.0f, 0.0f, -1.0f };
        glm::vec3 m_up {0.0f, 1.0f, 0.0f}; // Hardcode for now, since looking movement is on xy plane
        
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;
    };
}