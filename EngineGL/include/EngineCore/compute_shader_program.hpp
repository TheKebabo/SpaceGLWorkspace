#pragma once

#include "shader_program.hpp"

namespace Engine
{
    class ComputeShaderProgram : public ShaderProgram
    {
    public:
        ComputeShaderProgram(const char* computePath);
    };
}