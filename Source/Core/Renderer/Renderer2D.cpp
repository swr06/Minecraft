#include "Renderer2D.h"

namespace Minecraft
{
    Renderer2D::Renderer2D() : m_VBO(GL_ARRAY_BUFFER)
    {
        GLuint index_buffer[6] = { 0,1,3,1,2,3 };

        m_VAO.Bind();
        m_VBO.Bind();
        m_IBO.Bind();
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, 0, 5 * sizeof(GLfloat), (void*)0);
        m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, 0, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        m_IBO.BufferData(6 * sizeof(GLuint), index_buffer, GL_STATIC_DRAW);
        m_VAO.Unbind();

        m_DefaultShader.CreateShaderProgramFromFile("Shaders/2DElementShaderVert.glsl", "Shaders/2DElementShaderFrag.glsl");
        m_DefaultShader.CompileShaders();
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, GLClasses::Texture* texture, OrthographicCamera* camera)
	{
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        const std::array<GLfloat, 8> texture_coords = texture->GetTextureCoords();

        float x, y, w, h;

        x = position.x;
        y = position.y;
        w = position.x + texture->GetWidth();
        h = position.y + texture->GetHeight();

        GLfloat Vertices[] = {
            w, y, 1.0f, texture_coords[0], texture_coords[1],
            w, h, 1.0f, texture_coords[2], texture_coords[3],
            x, h, 1.0f, texture_coords[4], texture_coords[5],
            x, y, 1.0f, texture_coords[6], texture_coords[7],
        };

        m_DefaultShader.Use();
        texture->Bind(1);
        m_DefaultShader.SetMatrix4("u_Projection", camera->GetProjectionMatrix(), 0);
        m_DefaultShader.SetMatrix4("u_View", camera->GetViewMatrix(), 0);
        m_DefaultShader.SetMatrix4("u_Model", glm::mat4(1.0f), 0);
        m_DefaultShader.SetInteger("u_Texture", 1, 0);
        
        // Draw the 2D quad 
        m_VAO.Bind();
        m_VBO.BufferData(20 * sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        m_VAO.Unbind();

        glEnable(GL_DEPTH_TEST);
	}
}