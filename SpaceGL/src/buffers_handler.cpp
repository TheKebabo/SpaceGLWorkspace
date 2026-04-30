#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "buffers_handler.hpp"

namespace SpaceGL
{
    BuffersHandler::BuffersHandler(std::vector<Body>& bodies)
    {
        initBodiesBuffers(bodies);
        initOrbitsBuffers(bodies);
    }

    void BuffersHandler::initBodiesBuffers(std::vector<Body>& bodies)
    {
        // CUBE VERTEX DATA
        // Each planetary "body" will be represented by a CUBE of its bounding box
        GLfloat bodiesBoundingVertices[] = {
            -1, -1, -1,  -1, -1, 1,   -1, 1, -1,   -1, 1, 1,
             1, -1, -1,   1, -1, 1,    1, 1, -1,    1, 1, 1
        };

        // CUBE INDEX DATA
        GLuint bodiesBoundingIndices[] = {
            // Left face (-X)
            0, 1, 3,   0, 3, 2,
            // Right face (+X)
            5, 4, 6,   5, 6, 7,
            // Bottom face (-Y)
            0, 4, 5,   0, 5, 1,
            // Top face (+Y)
            3, 7, 6,   3, 6, 2,
            // Back face (-Z)
            4, 0, 2,   4, 2, 6,
            // Front face (+Z)
            1, 5, 7,   1, 7, 3
        };
        
        // Create VBO and send vertex data
        glGenBuffers(1, &m_bodiesBoundariesVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_bodiesBoundariesVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, bodiesBoundingVertices, GL_STATIC_DRAW);

        // Create VAO
        glGenVertexArrays(1, &m_bodiesVAO); // // Generates the object and stores the resulting id in passed in integer
        glBindVertexArray(m_bodiesVAO); // Binds 'VAO' as current active vertex array object
        glEnableVertexAttribArray(0);   /// Enables vertex attribute at location = 0, since they are disabled by default
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);   // Describes to OpenGL how to interpet vertex POSITION data

        // Create EBO and send index data
        glGenBuffers(1, &m_bodiesBoundariesEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bodiesBoundariesEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, bodiesBoundingIndices, GL_STATIC_DRAW);

        // BODIES INSTANCE VERTEX DATA
        std::vector<BodyData> bodiesData;
        for (Body& body : bodies)
        {
            bodiesData.push_back(BodyData{body.pos().x, body.pos().y, body.pos().z, (float)body.radius()});
        }

        // Create VBO and send vertex data
        glGenBuffers(1, &m_bodiesDataVBO);
        updateBodiesPosBuffer(bodiesData);

        glBindVertexArray(m_bodiesVAO); // Binds 'VAO' as current active vertex array object
        glEnableVertexAttribArray(1);   /// Enables vertex attribute at location = 1, since they are disabled by default
        glBindBuffer(GL_ARRAY_BUFFER, m_bodiesDataVBO);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(BodyData), (void*)0);   // Describes to OpenGL how to interpet vertex POSITION data
        //Tell OpenGL this data is per-instance, not per-vertex
        glVertexAttribDivisor(1, 1);

        // Unbind VBO, VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BuffersHandler::initOrbitsBuffers(std::vector<Body>& bodies)
    {
        // QUAD VERTEX DATA
        // Each planetary orbit will be represented by a quad of its bounding box
        // This will need to be transformed to match each body's velocity and position
        GLfloat orbitsBoundingVertices[] = {
            -1.0f, -1.0f,  // 0: Bottom-Left
             1.0f, -1.0f,  // 1: Bottom-Right
             1.0f,  1.0f,  // 2: Top-Right
            -1.0f,  1.0f   // 3: Top-Left
        };

        // QUAD INDEX DATA
        GLuint orbitsBoundingIndices[] = {
            0, 1, 2,  // First Triangle (BL -> BR -> TR)
            0, 2, 3   // Second Triangle (BL -> TR -> TL)
        };

        // Create VAO, VBO, EBO
        glGenVertexArrays(1, &m_orbitsVAO);
        glGenBuffers(1, &m_orbitsBoundariesVBO);
        glGenBuffers(1, &m_orbitsBoundariesEBO);

        // Bind VAO
        glBindVertexArray(m_orbitsVAO); // Binds 'VAO' as current active vertex array object

        // Send vertex data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_orbitsBoundariesVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(orbitsBoundingVertices), orbitsBoundingVertices, GL_STATIC_DRAW);

        // Setup VAO attributes
        glEnableVertexAttribArray(0);   /// Enables vertex attribute at location = 0, since they are disabled by default
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);   // Describes to OpenGL how to interpet vertex POSITION data

        // Send index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_orbitsBoundariesEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(orbitsBoundingIndices), orbitsBoundingIndices, GL_STATIC_DRAW);

        // ORBITS INSTANCE VERTEX DATA
        std::vector<glm::mat4> orbitsData;
        for (Body& body : bodies)
        {
            orbitsData.push_back(glm::mat4(1.0f));
        }

        // Create VBO and send vertex data
        glGenBuffers(1, &m_orbitsDataVBO);
        updateOrbitsPosBuffer(orbitsData);

        glBindVertexArray(m_orbitsVAO); // Binds 'VAO' as current active vertex array object
        glBindBuffer(GL_ARRAY_BUFFER, m_orbitsDataVBO);
        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(i + 1);   /// Enables vertex attribute at locations = 1 to 4
            glVertexAttribPointer(i + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));   // Describes to OpenGL how to interpet vertex POSITION data
            // Tell OpenGL this data is per-instance, not per-vertex
            glVertexAttribDivisor(i + 1 , 1);
        }

        // Unbind VBO, VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BuffersHandler::updateBodiesPosBuffer(std::vector<BodyData>& newBodiesData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bodiesDataVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BodyData) * newBodiesData.size(), newBodiesData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BuffersHandler::updateOrbitsPosBuffer(std::vector<glm::mat4>& newOrbitsData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_orbitsDataVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * newOrbitsData.size(), newOrbitsData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}