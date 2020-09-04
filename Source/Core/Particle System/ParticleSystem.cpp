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
			glm::mat4 combine_matrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1, 1, 1));

			view_matrix = view_matrix * combine_matrix;

			glm::mat4 proj_matrix = camera->GetProjectionMatrix();
			glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(particle.p_Position));

			model_matrix[0][0] = view_matrix[0][0];
			model_matrix[0][1] = view_matrix[1][0];
			model_matrix[0][2] = view_matrix[2][0];
			model_matrix[1][0] = view_matrix[0][1];
			model_matrix[1][1] = view_matrix[1][1];
			model_matrix[1][2] = view_matrix[2][1];
			model_matrix[2][0] = view_matrix[0][2];
			model_matrix[2][1] = view_matrix[1][2];
			model_matrix[2][2] = view_matrix[2][2];

			model_matrix = model_matrix * glm::scale(glm::mat4(1.0f), glm::vec3(particle.GetScale()));

			m_ParticleShader.Use();
			m_ParticleShader.SetMatrix4("u_ViewMatrix", view_matrix);
			m_ParticleShader.SetMatrix4("u_ProjectionMatrix", proj_matrix); 
			m_ParticleShader.SetMatrix4("u_ModelMatrix", model_matrix);
			VAO.Bind();
			DebugGLFunction(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
			VAO.Unbind();
		}

		ParticleEmitter::ParticleEmitter()
		{

		}

		void ParticleEmitter::EmitParticlesAt(float lifetime, int num_particles, const glm::vec3& origin, const glm::vec3& extent, const glm::vec3& vel)
		{
			static Random random;

			for (int i = 0; i < num_particles; i++)
			{
				// Increment the x and z by a random amount
				float ix = random.UnsignedInt(extent.x) * 0.1f;
				float iy = random.UnsignedInt(extent.y) * 0.1f;
				float iz = random.UnsignedInt(extent.z) * 0.1f;
				ParticleDirection dir = ParticleDirection::left;

				if (random.Int(4) % 2 == 0) 
				{ 
					ix = -ix;
					iz = -iz; 
				}

				if (random.Int(2) == 0)
				{
					dir = ParticleDirection::right;
				}

				Particle p(glm::vec3(origin.x + ix, origin.y + iy, origin.z + iz), vel, lifetime, 0.1f, dir);
				m_Particles.push_back(p);
			}
		}

		void ParticleEmitter::OnUpdateAndRender(FPSCamera* camera)
		{
			for (int i = 0; i < m_Particles.size(); i++)
			{
				if (!m_Particles[i].p_IsAlive)
				{
					continue;
				}

				m_Particles[i].OnUpdate();
				m_Renderer.RenderParticle(m_Particles[i], camera);
			}
		}

		void ParticleEmitter::CleanUpList()
		{
			for (int i = 0; i < m_Particles.size(); i++)
			{
				if (!m_Particles[i].p_IsAlive)
				{
					m_Particles.erase(m_Particles.begin() + i);
				}
			}
		}

	}
}
