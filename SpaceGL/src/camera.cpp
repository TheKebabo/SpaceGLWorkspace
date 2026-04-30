#include <iostream>
#include "camera.hpp"

namespace SpaceGL
{
    void Camera::move(MouseDir d, float dt)
    {
        float moveUnit = 15.0f * dt;
        switch (d)
        {
            case FORWARD:
                {
                    glm::vec3 forward = m_front;
                    forward.y = 0; // Only want to move in XZ plane
                    m_pos += moveUnit * glm::normalize(forward);
                }
                break;
            case BACK:
                {
                    glm::vec3 backward = -m_front;
                    backward.y = 0; // Only want to move in XZ plane
                    m_pos += moveUnit * glm::normalize(backward);
                }
                break;
            case RIGHT:
                {
                    glm::vec3 right = glm::cross(m_front, m_up);
                    m_pos += moveUnit * right;
                }
                break;
            case LEFT:
                {
                    glm::vec3 left = glm::cross(m_front, -m_up);
                    m_pos += moveUnit * left;
                }
                break;
            case UP:
                m_pos += moveUnit * m_up;
                break;
            case DOWN:
                m_pos -= moveUnit * m_up;
                break;
        }
    }

    void Camera::updateYaw(float dYaw)
    {
        if (dYaw == 0) return;
        m_yaw += dYaw;
        m_calcFront();
    }

    void Camera::updatePitch(float dPitch)
    {
        if (dPitch == 0) return;
        m_pitch += dPitch;
        m_pitch = std::max(std::min(m_pitch, 89.0f), -89.0f);
        m_calcFront();
    }

    void Camera::m_calcFront()
    {
        m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front.y = sin(glm::radians(m_pitch));
        m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(m_front);
    }
}