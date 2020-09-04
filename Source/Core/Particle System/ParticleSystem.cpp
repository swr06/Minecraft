#include "ParticleSystem.h"

namespace Minecraft
{
	namespace ParticleSystem
	{
		ParticleRenderer::ParticleRenderer() : m_VBO(GL_ARRAY_BUFFER)
		{
			m_ParticleShader.CreateShaderProgramFromFile("Shaders/ParticleVert.glsl", "Shaders/ParticleFrag.glsl");
			m_ParticleShader.CompileShaders();

			int index_size = 12000 ; 
			int index_offset = 0;

			GLuint* IndexBuffer = nullptr;
			IndexBuffer = new GLuint[index_size * 6];

			for (size_t i = 0; i < index_size; i += 6)
			{
				IndexBuffer[i] = 0 + index_offset;
				IndexBuffer[i + 1] = 1 + index_offset;
				IndexBuffer[i + 2] = 2 + index_offset;
				IndexBuffer[i + 3] = 1 + index_offset;
				IndexBuffer[i + 4] = 2 + index_offset;
				IndexBuffer[i + 5] = 3 + index_offset;

				index_offset = index_offset + 4;
			}

			m_IBO.Bind();
			m_IBO.BufferData(index_size * 6 * sizeof(GLuint), IndexBuffer, GL_STATIC_DRAW);
			m_IBO.Unbind();
			m_VAO.Bind();
			m_IBO.Bind();
			m_VBO.Bind();
			m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, position));
			m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, texture_coords));
			m_VAO.Unbind();

			delete[] IndexBuffer;
		}

		void ParticleRenderer::StartParticleRender()
		{
			m_ParticleVertices.clear();
		}

		void ParticleRenderer::RenderParticle(const Particle& particle, FPSCamera* camera)
		{
			glm::mat4 view_matrix = camera->GetViewMatrix();
			glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(particle.p_Position));
			ParticleVertex v1, v2, v3, v4;

			// Use the transpose of the view matrix to get rid of the rotation 
			// So that the particles always face the camera
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

			const std::array<uint16_t, 8>& texture_coords = BlockDatabase::GetBlockTexture(particle.p_BlockType, BlockFaceType::top);

			v1.position = model_matrix * glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
			v2.position = model_matrix * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			v3.position = model_matrix * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			v4.position = model_matrix * glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

			v1.texture_coords = glm::vec2(texture_coords[0], texture_coords[1]);
			v2.texture_coords = glm::vec2(texture_coords[2], texture_coords[3]);
			v3.texture_coords = glm::vec2(texture_coords[4], texture_coords[5]);
			v4.texture_coords = glm::vec2(texture_coords[6], texture_coords[7]);

			m_ParticleVertices.push_back(v1);
			m_ParticleVertices.push_back(v2);
			m_ParticleVertices.push_back(v3);
			m_ParticleVertices.push_back(v4);
		}

		void ParticleRenderer::EndParticleRender(FPSCamera* camera, GLClasses::Texture* texture_atlas)
		{
			if (m_ParticleVertices.size() > 0)
			{
				m_ParticleShader.Use();
				m_ParticleShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection(), 0);
				m_ParticleShader.SetInteger("u_Texture", 0, 0);
				texture_atlas->Bind(0);
				m_VAO.Bind();
				m_VBO.BufferData(m_ParticleVertices.size() * sizeof(ParticleVertex), &m_ParticleVertices.front(), GL_STATIC_DRAW);
				glDrawElements(GL_TRIANGLES, (m_ParticleVertices.size()) * 6, GL_UNSIGNED_INT, 0);
				m_VAO.Unbind();
			}

			m_ParticleVertices.clear();

			glUseProgram(0);
		}

		ParticleEmitter::ParticleEmitter()
		{

		}

		void ParticleEmitter::EmitParticlesAt(float lifetime, int num_particles, const glm::vec3& origin, const glm::vec3& extent,
			const glm::vec3& vel, BlockType block)
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
				p.p_BlockType = block;
				m_Particles.push_back(p);
			}
		}

		void ParticleEmitter::OnUpdateAndRender(FPSCamera* camera, GLClasses::Texture* atlas)
		{
			m_Renderer.StartParticleRender();

			for (int i = 0; i < m_Particles.size(); i++)
			{
				if (!m_Particles[i].p_IsAlive)
				{
					continue;
				}

				m_Particles[i].OnUpdate();
				m_Renderer.RenderParticle(m_Particles[i], camera);
			}

			m_Renderer.EndParticleRender(camera, atlas);
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
