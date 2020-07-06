#include "CubeRenderer.h"

// A basic cube renderer that was used to test the basic idea
// This is not used internally

namespace Minecraft
{
	CubeRenderer::CubeRenderer() : m_VBO(GL_ARRAY_BUFFER)
	{
        m_DefaultShader.CreateShaderProgramFromFile("Shaders/vertex.glsl", "Shaders/fragment.glsl");
        m_DefaultShader.CompileShaders();

        float vertices[] = {

            // back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            // front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            // left
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            // right
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             // bottom
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            // top
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        m_VAO.Bind();
        m_VBO.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        m_VAO.Unbind();
	}

	CubeRenderer::~CubeRenderer()
	{

	}

    void CubeRenderer::RenderCube(const glm::vec3& position, Texture* texture, float rotation, const glm::mat4& projection, const glm::mat4& view, Shader* shader)
	{
        Shader* use_shader;

        if (shader == nullptr)
        {
            use_shader = &m_DefaultShader;
        }

        else
        {
            use_shader = shader;
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.5f, 0.5f));

        use_shader->Use();
        use_shader->SetMatrix4("u_Model", model);
        use_shader->SetMatrix4("u_View", view);
        use_shader->SetMatrix4("u_Projection", projection);

        m_VAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_VAO.Unbind();
	}
}