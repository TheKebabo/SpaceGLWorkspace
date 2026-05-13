#pragma once

#include <glad.h>
#include <glm/glm.hpp>

namespace SpaceGL
{
    struct BodyData {
        glm::vec3 pos;
        float radius = 0.0f;
        float texIndex = -1.0f;
    };

    // Planetary body class
    class Body
    {
    public:
        Body() {}
        Body(glm::vec3 initPos, glm::vec3 initVec, double rad, double mass, size_t t_texIndex, size_t t_specularIndex)
        {
            m_pos = initPos;
            m_vel = initVec;
            m_radius = rad;
            m_mass = mass;
            m_tex = t_tex;
            m_specularTex = t_specular;
        }
        glm::vec3 pos() { return m_pos; }
        void setPos(glm::vec3 p) { m_pos = p; }
        void updatePos(glm::vec3 p) { m_pos += p; }
        glm::vec3 vel() { return m_vel; }
        void updateVel(glm::vec3 v) { m_vel += v; }
        glm::vec3 acc() { return m_acc; }
        void setAcc(glm::vec3 a) { m_acc = a; }
        void updateAcc(glm::vec3 a) { m_acc += a; }
        double radius() { return m_radius; }
        double mass() { return m_mass; }
        GLuint texIndex() { return m_texIndex; }
        GLuint specularTexIndex() { return m_specularTexIndex; }

    protected:
        glm::vec3 m_pos{0.0f, 0.0f, 0.0f};
        glm::vec3 m_vel{0.0f, 0.0f, 0.0f};
        glm::vec3 m_acc{0.0f, 0.0f, 0.0f};
        
        double m_radius = 1.0;
        double m_mass = 1.0;
        
        size_t m_texIndex;
        size_t m_specularTexIndex;
    };
}