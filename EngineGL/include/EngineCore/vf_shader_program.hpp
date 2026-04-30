#pragma once

#include "shader_program.hpp"

namespace Engine
{
    // Vertex / fragment shader program
    class VFShaderProgram : public ShaderProgram
    {
    public: 
        VFShaderProgram(const char* vertexPath, const char* fragmentPath);
    protected:
        const char* _vertexPath;
        const char* _fragmentPath;
        void checkLinkErrors();
    };
}