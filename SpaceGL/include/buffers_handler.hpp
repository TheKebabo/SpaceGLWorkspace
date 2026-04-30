#pragma once

#include <vector>
#include <glad.h>
#include "body.hpp"

namespace SpaceGL
{
    struct BodyData {
        float x = 0.0f, y = 0.0f, z = 0.0f;  // Position
        float radius = 0.0f;   // Radius
    };

    // Will handle sending data to and from GPU
    class BuffersHandler
    {
    public:
        BuffersHandler(std::vector<Body>& bodies);

        void initBodiesBuffers(std::vector<Body>& bodies);
        void updateBodiesPosBuffer(std::vector<BodyData>& newBodiesData);
        void initOrbitsBuffers(std::vector<Body>& bodies);
        void updateOrbitsPosBuffer(std::vector<glm::mat4>& newOrbitsData);
        GLuint bodiesVAO() { return m_bodiesVAO; }
        GLuint orbitsVAO() { return m_orbitsVAO; }
        GLuint bodiesBoundariesVBO() { return m_bodiesBoundariesVBO; }

    private:
        GLuint m_bodiesVAO;
        GLuint m_bodiesBoundariesVBO;
        GLuint m_bodiesBoundariesEBO;
        GLuint m_bodiesDataVBO;

        GLuint m_orbitsVAO;
        GLuint m_orbitsBoundariesEBO;
        GLuint m_orbitsBoundariesVBO;
        GLuint m_orbitsDataVBO;
    };
}