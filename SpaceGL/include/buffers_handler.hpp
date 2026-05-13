#pragma once

#include <vector>
#include <glad.h>
#include "body.hpp"
#include "resource_handler.hpp"

namespace SpaceGL
{
    // struct OrbitData {
    //     // alignas(16) glm::vec3 normal;  For SSBO
    //     glm::vec3 normal;
    //     // alignas(16) glm::vec3 periapsisDir;
    //     glm::vec3 periapsisDir;
    //     float a; // Semi-major axis
    //     float e; // Eccentricity
    // };

    // Will handle sending data to and from GPU
    class BuffersHandler
    {
    public:
        BuffersHandler(std::vector<Body>& bodies);

        void initBodiesBuffers(std::vector<Body>& bodies);
        void updateBodiesPosBuffer(std::vector<BodyData>& newBodiesData);
        void initOrbitsBuffers(std::vector<Body>& bodies);
        void updateOrbitsPosBuffer(std::vector<glm::mat4>& newOrbitsData);
        GLuint genTexture(std::string path);
        GLuint genTextureArray(std::vector<std::string> paths, size_t width, size_t height);
        void initSkybox();
        GLuint bodiesVAO() { return m_bodiesVAO; }
        GLuint orbitsVAO() { return m_orbitsVAO; }
        GLuint skyboxVAO() { return m_skyboxVAO; }
        GLuint skyboxTexture() { return m_skyboxTexture; }

    private:
        GLuint m_bodiesVAO;
        GLuint m_orbitsVAO;
        GLuint m_skyboxVAO;
        
        GLuint m_bodiesDataVBO;
        GLuint m_orbitsDataVBO;
        
        GLuint m_skyboxTexture;

        ResourceHandler m_resourceHandler;
    };
}