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
    Simulator::Simulator(int winWidth, int winHeight, double mouseDX, double mouseDY, int nBodies)
    {
        // for (int i = 0; i < nBodies; ++i)
        // {
        //     bodies.push_back(Body(glm::vec3(0.0f), glm::vec3(0.0), 5.0, 50.0));
        // };
        bodies.push_back(Body(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(5.0), 10.0, 355.0));
        bodies.push_back(Body(glm::vec3(0.0f, -25.0f, 0.0f), glm::vec3(4.33, 0.0, 0.0), 2.0, 2.0));
        bodies.push_back(Body(glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(-3.44f, 0.0f, 0.0f), 2.0, 2.0));

        m_buffersHandler.emplace(bodies);
        m_camera.emplace(0.0f, 0.0f, 70.0f);
        m_renderer.emplace(winWidth, winHeight, *m_camera, mouseDX, mouseDY);
    }

    void Simulator::reset()
    {
        m_buffersHandler->initBodiesBuffers(bodies);
        m_buffersHandler->initOrbitsBuffers(bodies);
    }

    void Simulator::update(double mouseDX, double mouseDY, float dt)
    {
        float G = 1.0f;

        // Reset accs
        for (Body& a : bodies) a.setAcc(glm::vec3(0.0f));
        
        // Calc new accs
        for (size_t i = 0; i < bodies.size(); ++i)
        {
            for (size_t j = i+1; j < bodies.size(); ++j)
            {
                Body& a = bodies[i];
                Body& b = bodies[j];

                // Calc. new acceleration a = (G * M / r^2) * (r / |r|) = (G * M / |r|^3) * r
                glm::vec3 r = b.pos() - a.pos();

                float softening = 0.01; // Stop force exploding with small r
                float dist = glm::length(r) + softening;
                
                if (dist < 0.01f) continue; // Safety check
                
                float commonAccVal = G / (dist*dist*dist);
                a.updateAcc(commonAccVal * (float)b.mass() * r);
                b.updateAcc(commonAccVal * (float)a.mass() * -r);
                // std::cout << b.acc().x << ", " << b.acc().y << ", " << b.acc().z << std::endl;
            }
        }
        
        std::vector<BodyData> newBodiesData;
        std::vector<glm::mat4> newOrbitsData;

        // Calc new vels and poses
        for (Body& body : bodies)
        {
            glm::vec3 dVel = dt * body.acc();
            body.updateVel(dVel);
            glm::vec3 dPos = dt * body.vel();
            body.updatePos(dPos);
            newBodiesData.push_back(BodyData{body.pos().x, body.pos().y, body.pos().z, (float)body.radius()});

            // ADD ORBIT DATA
            glm::vec3 orbitDir = glm::normalize(body.acc());

            glm::mat4 instanceModel = glm::mat4(1.0f);
            
            // |a| = v^2/r so r = v^2/|a|
            float orbitR = glm::dot(body.vel(), body.vel()) / glm::length(body.acc());
            glm::vec3 orbitCentre = body.pos() + orbitR * orbitDir;

            // Translate by orbit centre
            instanceModel = glm::translate(instanceModel, orbitCentre);

            // Rotate model UP vector so it is pointing in orbit direction
            glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), orbitDir);
            if (glm::length(axis) > 0.001f) // Check if dir is (nearly) straight up or down
            {
                float angle = std::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), orbitDir));
                instanceModel = glm::rotate(instanceModel, angle, axis);
            }

            // Scale by radius of orbit - from radius to 1 to radius orbitR
            instanceModel = glm::scale(instanceModel, glm::vec3(orbitR, orbitR, 1.0f));

            newOrbitsData.push_back(instanceModel);
        }
        
        m_renderer->updateCamPosUniform(*m_camera, mouseDX, mouseDY);
        m_buffersHandler->updateBodiesPosBuffer(newBodiesData);
        m_buffersHandler->updateOrbitsPosBuffer(newOrbitsData);
        m_renderer->renderBodies(m_buffersHandler->bodiesVAO(), bodies.size());
        m_renderer->renderOrbits(m_buffersHandler->orbitsVAO(), bodies.size());
    }
}