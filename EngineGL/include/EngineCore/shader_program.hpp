#pragma once

#include <string>
#include <sstream>
#include <glad.h>
#include <unordered_map>

#define INVALID_UNIFORM_LOC 0xffffffff;

namespace Engine
{
    class ShaderProgram // ABSTRACT CLASS
    {
    public:
        unsigned ID;

        ShaderProgram() {}
        // Destructor
        ~ShaderProgram();
        // Activate the shader program
        void use();
        // Query uniform location
        GLint getUniformLocation(const std::string& name);
        // Utility uniform functions
        void setBool_w_Name(const std::string &name, bool value);  
        void setBool_w_Loc(GLint location, bool value) const;
        void setInt_w_Name(const std::string &name, int value);   
        void setInt_w_Loc(GLint location, int value) const;
        void setFloat_w_Name(const std::string &name, float value);
        void setFloat_w_Loc(GLint location, float value) const;
        void set3Floats_w_Name(const std::string &name, float value0, float value1, float value2);
        void set3Floats_w_Loc(GLint location, float value0, float value1, float value2) const;
        void set4Floats_w_Name(const std::string &name, float value0, float value1, float value2, float value3);   
        void set4Floats_w_Loc(GLint location, float value0, float value1, float value2, float value3) const;   
        void setMat4_w_Name(const std::string &name, GLboolean transpose, const GLfloat* value);
        void setMat4_w_Loc(GLint location, GLboolean transpose, const GLfloat* value) const;
    protected:
        std::unordered_map<std::string, GLint> m_UniformLocationCache;

        unsigned readAndCompileShaderFile(const char* shaderPath, unsigned& shaderID, std::string shaderType);
        void checkCompileErrors(unsigned& shaderID, std::string shaderType);
        virtual void checkLinkErrors();
    };
}