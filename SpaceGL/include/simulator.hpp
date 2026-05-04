#pragma once

#include <vector>
#include <optional>
#include "body.hpp"
#include "buffers_handler.hpp"
#include "renderer.hpp"

namespace SpaceGL
{
    class Simulator
    {
    public:
        Simulator(int winWidth, int winHeight,double mouseDX, double mouseDY, int nBodies);
        void reset();
        void update( double mouseDX, double mouseDY, float dt);

        Camera& camera()
        {
            if (!m_camera.has_value()) throw std::runtime_error("Camera object is empty.");
            return *m_camera;
        }

    private:
        std::vector<Body> bodies;
        std::optional<BuffersHandler> m_buffersHandler;
        std::optional<Renderer> m_renderer;
        std::optional<Camera> m_camera;

        void m_calcOrbit(Body& body, Body& central, std::vector<glm::mat4>& newOrbitsData);
    };
}