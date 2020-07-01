#include "Skybox.h"

namespace Minecraft
{
	Skybox::Skybox() : m_VBO(GL_ARRAY_BUFFER)
	{
        GLfloat SkyboxVertices[] =
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        m_CubeMap.CreateCubeTextureMap({
            "Resources/Skybox/right.jpg",
            "Resources/Skybox/left.jpg",
            "Resources/Skybox/top.jpg",
            "Resources/Skybox/bottom.jpg",
            "Resources/Skybox/front.jpg",
            "Resources/Skybox/back.jpg"
            });

        m_SkyboxShader.CreateShaderProgramFromFile("Shaders/SkyboxVertex.glsl", "Shaders/SkyboxFrag.glsl");
        m_SkyboxShader.CompileShaders();

        m_VAO.Bind();
        m_VBO.BufferData(sizeof(SkyboxVertices), SkyboxVertices, GL_STATIC_DRAW);
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_VAO.Unbind();
	}

    void Skybox::RenderSkybox(FPSCamera* camera)
    {
        glDepthMask(GL_FALSE);
        m_SkyboxShader.Use();

        m_SkyboxShader.SetMatrix4("u_Projection", camera->GetProjectionMatrix());
        m_SkyboxShader.SetMatrix4("u_View", glm::mat4(glm::mat3(camera->GetViewMatrix())));
        m_SkyboxShader.SetInteger("u_Skybox", 0);

        m_VAO.Bind();
        glActiveTexture(GL_TEXTURE0);
        DebugGLFunction(glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.GetID()));
        DebugGLFunction(glDrawArrays(GL_TRIANGLES, 0, 36));
        glDepthMask(GL_TRUE);

        m_VAO.Unbind();
    }
}
