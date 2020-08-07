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
            "Resources/Skybox/right.bmp",
            "Resources/Skybox/left.bmp",
            "Resources/Skybox/top.bmp",
            "Resources/Skybox/bottom.bmp",
            "Resources/Skybox/front.bmp",
            "Resources/Skybox/back.bmp"
            });

        m_SkyboxShader.CreateShaderProgramFromFile("Shaders/SkyboxVertex.glsl", "Shaders/SkyboxFrag.glsl");
        m_SkyboxShader.CompileShaders();

        m_VAO.Bind();
        m_VBO.BufferData(sizeof(SkyboxVertices), SkyboxVertices, GL_STATIC_DRAW);
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_VAO.Unbind();
	}

    void Skybox::RenderSkybox(FPSCamera* camera, const glm::vec4& sun_pos)
    {
        glDepthMask(GL_FALSE);
        m_SkyboxShader.Use();

        m_SkyboxShader.SetMatrix4("u_Projection", camera->GetProjectionMatrix());
        m_SkyboxShader.SetMatrix4("u_View", glm::mat4(glm::mat3(camera->GetViewMatrix())));
        m_SkyboxShader.SetInteger("u_Skybox", 0);
        m_SkyboxShader.SetFloat("u_SunPositionY", sun_pos.y);

        m_VAO.Bind();
        glActiveTexture(GL_TEXTURE0);
        DebugGLFunction(glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.GetID()));
        DebugGLFunction(glDrawArrays(GL_TRIANGLES, 0, 36));

        m_VAO.Unbind();

        glDepthMask(GL_TRUE);
    }
}
