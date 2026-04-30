#pragma once

#include <optional>
#include <EngineCore/vf_shader_program.hpp>
#include "camera.hpp"

namespace SpaceGL
{
    class Renderer
    {
    public:
        Renderer(int winWidth, int winHeight, Camera& cam, double mouseDX, double mouseDY);
        void setUniforms(Camera& cam, double mouseDX, double mouseDY);
        void updateCamPosUniform(Camera& cam, double mouseDX, double mouseDY);
        void renderBodies(GLuint VAO, GLsizei bodiesCount);
        void renderOrbits(GLuint VAO, GLsizei orbitsCount);

    private:
        int m_widthPX; int m_heightPX;
        std::optional<Engine::VFShaderProgram> m_bodiesShader;
        std::optional<Engine::VFShaderProgram> m_orbitsShader;
    };
}