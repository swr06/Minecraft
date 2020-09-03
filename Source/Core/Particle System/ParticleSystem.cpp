#include "ParticleSystem.h"

namespace Minecraft
{
	namespace ParticleSystem
	{
		ParticleRenderer::ParticleRenderer()
		{
			m_ParticleShader.CreateShaderProgramFromFile("Shaders/ParticleVert.glsl", "Shaders/ParticleFrag.glsl");
			m_ParticleShader.CompileShaders();
		}

		void ParticleRenderer::RenderParticle(const Particle& particle, FPSCamera* camera)
		{
			GLuint indices[6] = { 0,1,2,1,2,3 };
			GLfloat vertices[] = { 
				0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f };

			GLClasses::VertexBuffer VBO(GL_ARRAY_BUFFER);
			GLClasses::VertexArray VAO;
			GLClasses::IndexBuffer IBO;
			VAO.Bind();
			VBO.Bind();
			IBO.Bind();
			VBO.VertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(GLfloat) * 3, 0);
			VBO.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
			IBO.BufferData(sizeof(indices), indices, GL_STATIC_DRAW);
			VAO.Unbind();

			glm::mat4 view_matrix = camera->GetViewMatrix();
			view_matrix[0][0] = 1.0f;
			view_matrix[0][1] = 0.0f;
			view_matrix[0][2] = 0.0f;
			view_matrix[1][0] = 0.0f;
			view_matrix[1][1] = 1.0f;
			view_matrix[1][2] = 0.0f;
			view_matrix[2][0] = 0.0f;
			view_matrix[2][1] = 0.0f;
			view_matrix[2][2] = 1.0f;

			glm::mat4 proj_matrix = camera->GetProjectionMatrix();
			glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(particle.p_Position));

			m_ParticleShader.Use();
			m_ParticleShader.SetMatrix4("u_ViewMatrix", view_matrix);
			m_ParticleShader.SetMatrix4("u_ProjectionMatrix", proj_matrix); 
			m_ParticleShader.SetMatrix4("u_ModelMatrix", model_matrix);
			VAO.Bind();
			DebugGLFunction(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
			VAO.Unbind();
		}

	}
}
