#include <iostream>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "simulator.hpp"
#include "buffers_handler.hpp"
#include "renderer.hpp"

namespace SpaceGL
{
    float G = 15.0f;

    Simulator::Simulator(int winWidth, int winHeight, double mouseDX, double mouseDY, int nBodies)
    {
        std::string assetsPath = std::string(ASSETS_PATH);
        std::vector<std::string> texPaths = {
            assetsPath + "2k_earth_daymap.jpg",
            assetsPath + "2k_jupiter.jpg"
        };
        std::vector<std::string> specularTexPaths = {
            assetsPath + "2k_earth_specular_map.png",
            assetsPath + "2k_lower_specular_map.png"
        };

        bodies.push_back(Body(glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 5000.0f, 1, 1));
        bodies.push_back(Body(glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 22.0f, 0.0f), 3.0f, 100.0f, 0, 0));
        bodies.push_back(Body(glm::vec3(-160.0f, 0.0f, 0.0f), glm::vec3(0.0f, 23.5f, 0.0), 3.0f, 50.0f, 0, 0));

        m_buffersHandler.emplace(bodies, texPaths, specularTexPaths, 2048, 1024);
        m_camera.emplace(0.0f, 0.0f, 100.0f);
        m_renderer.emplace(winWidth, winHeight, *m_camera, mouseDX, mouseDY);
    }

    void Simulator::reset()
    {
        m_buffersHandler->initBodiesBuffers(bodies);
        m_buffersHandler->initOrbitsBuffers(bodies);
    }

    void Simulator::update(double mouseDX, double mouseDY, float dt)
    {
        // Reset accs
        for (Body& a : bodies) a.setAcc(glm::vec3(0.0f));
         
        int strongestBodyForEach[bodies.size()]; // For each body, there will be one other with strongest pull
        float strongestForceForEach[bodies.size()];
        for (size_t i = 0; i < bodies.size(); ++i)
        {
            strongestBodyForEach[i] = i;
            strongestForceForEach[i] = 0.0;
        }

        // Calc new accs

        for (size_t i = 0; i < bodies.size(); ++i)
        {
            for (size_t j = i+1; j < bodies.size(); ++j)
            {
                Body& A = bodies[i];
                Body& B = bodies[j];

                glm::vec3 r = B.pos() - A.pos();

                // Calc. new acceleration a = (G * M / r^2) * (r / |r|) = (G * M / |r|^3) * r
                float softening = 0.01f; // Stop force exploding with small r
                float dist = glm::length(r) + softening;
                
                if (dist < 0.01f) continue; // Safety check
                
                float commonAccVal = G / (dist*dist*dist);
                glm::vec3 forceA = commonAccVal * (float)B.mass() * r;
                glm::vec3 forceB = commonAccVal * (float)A.mass() * -r;

                A.updateAcc(forceA);
                B.updateAcc(forceB);

                float forceAVal = glm::length2(forceA);
                if (forceAVal > strongestForceForEach[i])
                {
                    // std::cout << A.mass() << ": " << B.mass() << std::endl;
                    strongestBodyForEach[i] = j;
                    strongestForceForEach[i] = forceAVal;
                }

                float forceBVal = glm::length2(forceB);
                if (forceBVal > strongestForceForEach[j])
                {
                    // std::cout << A.mass() << ": " << B.mass() << std::endl;
                    strongestBodyForEach[j] = i;
                    strongestForceForEach[j] = forceBVal;
                }
            }
        }
        
        std::vector<BodyData> newBodiesData;
        std::vector<glm::mat4> newOrbitsData;

        // Calc new vels and poses, and new orbit data
        for (size_t i = 0; i < bodies.size(); ++i)
        {
            Body& body = bodies[i];

            // UPDATE VEL AND POS
            glm::vec3 dVel = dt * body.acc();
            body.updateVel(dVel);
            glm::vec3 dPos = dt * body.vel();
            body.updatePos(dPos);
            newBodiesData.push_back(BodyData{body.pos(), (float)body.radius(), body.texIndices()});
    
            // CALCULATE ORBIT SHAPE
            Body& central = bodies[strongestBodyForEach[i]];

            // ADD ORBIT DATA
            m_calcOrbit(body, central, newOrbitsData);
        }
        
        m_renderer->updateCamPosUniform(*m_camera, mouseDX, mouseDY);
        m_buffersHandler->updateBodiesPosBuffer(newBodiesData);
        m_buffersHandler->updateOrbitsPosBuffer(newOrbitsData);
        m_renderer->renderScene(m_buffersHandler->bodiesVAO(), m_buffersHandler->bodiesTexArray(), m_buffersHandler->bodiesSpecularTexArray(),
                                bodies.size(),
                                m_buffersHandler->orbitsVAO(),
                                m_buffersHandler->skyboxVAO(), m_buffersHandler->skyboxTexture());
    }

    void Simulator::m_calcOrbit(Body& body, Body& central, std::vector<glm::mat4>& newOrbitsData)
    {
        // GET REQUIRED DATA
        glm::vec3 v = body.vel() - central.vel();
        glm::vec3 r = body.pos() - central.pos();
        double M = central.mass();
        float mew = G * M;

        // Calculate orbital energy (KE - PE)
        double E = 0.5 * glm::length2(v) - (mew) / (glm::length(r) + 0.001);
        
        // Find eccentricity
        glm::vec3 eVec = (1.0f / (float)mew) *
            ((glm::length2(v) - mew/glm::length(r)) * r - glm::dot(r, v) * v);
        float e2 = glm::length2(eVec);

        glm::vec3 periapsisDir;
        if (e2 < 0.000001f)
        {
            // If it's a circle, any direction works as the "periapsis"
            periapsisDir = glm::vec3(1.0f, 0.0f, 0.0f); 
        } else {
            periapsisDir = (1.0f / (float)sqrt(e2)) * eVec;
        }

        float a = -(G * M) / (2 * E); // Semi-major axis
        float b = a * sqrt(1.0f - e2); // Semi-minor axis

        // Normal of orbit is angular momentum
        glm::vec3 h = glm::cross(r, v);
        glm::vec3 normal = glm::normalize(h); // Get 'Z-axis' of orbit

        glm::vec3 Q = glm::cross(normal, periapsisDir); // Get 'Y-axis' of orbit

        // CONSTRUCT MODEL MATRIX
        glm::mat4 orbitModel(1.0f);

        // Translate to central body position
        orbitModel = glm::translate(orbitModel, central.pos());

        glm::mat4 rotation(1.0f);
        rotation[0] = glm::vec4(periapsisDir, 0.0f);
        rotation[1] = glm::vec4(Q, 0.0f);
        rotation[2] = glm::vec4(normal, 0.0f);

        orbitModel = orbitModel * rotation;

        // Offset so central body is at focus point
        float focalOffset = a*sqrt(e2);
        orbitModel = glm::translate(orbitModel, glm::vec3(-focalOffset, 0.0f, 0.0f));

        orbitModel = glm::scale(orbitModel, glm::vec3(a, b, 1.0f)); // This is done 1st

        newOrbitsData.push_back(orbitModel);
    }
}