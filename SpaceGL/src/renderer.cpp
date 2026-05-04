#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.hpp"

namespace SpaceGL
{
    Renderer::Renderer(int winWidth, int winHeight, Camera& cam, double mouseDX, double mouseDY)
    {
        m_bodiesShader.emplace(SHADER_PATH "sphereVert.vs", SHADER_PATH "sphereFrag.fs");
        m_orbitsShader.emplace(SHADER_PATH "ringVert.vs", SHADER_PATH "ringFrag.fs");

        m_widthPX = winWidth; m_heightPX = winHeight;
        glViewport(0, 0, m_widthPX, m_heightPX);
        setUniforms(cam, mouseDX, mouseDY);
        
        // glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DITHER);
    }

    void Renderer::setUniforms(Camera& cam, double mouseDX, double mouseDY)
    {
        if (!m_bodiesShader.has_value())
        {
            throw std::runtime_error("Bodies shader was not initialised");
        }
        
        updateCamPosUniform(cam, mouseDY, mouseDY);

        // Bodies
        float fov = glm::radians(45.0f);
        float near = 1.0f; float far = 10000.0f;
        glm::mat4 proj = glm::perspective(fov, (float)m_widthPX / (float)m_heightPX, near, far);
        m_bodiesShader->use();
        m_bodiesShader->setMat4_w_Name("proj", false, &proj[0][0]);
        
        glm::vec3 bodyCol(1.0f, 1.0f, 1.0f);
        m_bodiesShader->set3Floats_w_Name("bodyCol", bodyCol.x, bodyCol.y, bodyCol.z);
        
        // Orbits
        m_orbitsShader->use();
        m_orbitsShader->setMat4_w_Name("proj", false, &proj[0][0]);

        glm::vec3 orbitCol(1.0f, 1.0f, 1.0f);
        m_orbitsShader->set3Floats_w_Name("ringCol", orbitCol.x, orbitCol.y, orbitCol.z);
    }

    void Renderer::updateCamPosUniform(Camera& cam, double mouseDX, double mouseDY)
    {
        glm::vec3 camPos = cam.getPos();

        float sensitivity = 0.05f;
        cam.updateYaw((float)mouseDX * sensitivity);
        cam.updatePitch((float)mouseDY * sensitivity);

        glm::vec3 front = cam.getFront();
        glm::mat4 view = glm::lookAt(camPos, camPos + front, glm::vec3(0, 1, 0));
        
        m_bodiesShader->use();
        m_bodiesShader->set3Floats_w_Name("camPos", camPos.x, camPos.y, camPos.z);
        m_bodiesShader->setMat4_w_Name("view", false, &view[0][0]);
        
        m_orbitsShader->use();
        m_orbitsShader->setMat4_w_Name("view", false, &view[0][0]);
    }

    void Renderer::renderBodies(GLuint VAO, GLsizei bodiesCount)
    {
        if (!m_bodiesShader.has_value())
        {
            throw std::runtime_error("Bodies shader was not initialised");
        }

        m_bodiesShader->use();
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, bodiesCount);
        glBindVertexArray(0);
    }

    void Renderer::renderOrbits(GLuint VAO, GLsizei orbitsCount)
    {
        if (!m_orbitsShader.has_value())
        {
            throw std::runtime_error("Bodies shader was not initialised");
        }

        m_orbitsShader->use();
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, orbitsCount);
        glBindVertexArray(0);
    }
}