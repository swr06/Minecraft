#include "Renderer2D.h"

namespace Minecraft
{
    Renderer2D::Renderer2D() : m_VBO(GL_ARRAY_BUFFER)
    {
        GLuint index_buffer[6] = { 0,1,2,2,3,0 };

        m_VAO.Bind();
        m_VBO.Bind();
        m_IBO.Bind();
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, 0, 5 * sizeof(GLfloat), (void*)0);
        m_VBO.VertexAttribPointer(0, 2, GL_FLOAT, 0, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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

        float Vertices[] = {
             800.0f, 600.0f, 0.0f, texture_coords[0], texture_coords[1],
             800.0f, 0.0f,   0.0f, texture_coords[2], texture_coords[3], 
             0.0f,   0.0f,   0.0f, texture_coords[4], texture_coords[5], 
             0.0f,   600.0f, 0.0f, texture_coords[6], texture_coords[7]  
        };

        glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

        m_DefaultShader.Use();
        m_DefaultShader.SetMatrix4("u_ViewProjection", camera->GetViewProjectionMatrix(), 0);
        m_DefaultShader.SetMatrix4("u_Model", Model, 0);
        
        // Draw the 2D quad 
        m_VAO.Bind();
        m_VBO.BufferData(sizeof(Vertices), Vertices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        m_VAO.Unbind();

        glEnable(GL_DEPTH_TEST);
	}
}