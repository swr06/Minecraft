#pragma once

#include <iostream>

#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/IndexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/Texture.h"
#include "../Block.h"
#include "../BlockDatabase.h"
#include "../OpenGL Classes/GLDebug.h"
#include "../FpsCamera.h"

#include "../Particle System/Particle.h"
#include "../Utils/Random.h"

namespace Minecraft
{
	namespace ParticleSystem
	{
		struct ParticleVertex
		{
			glm::vec3 position;
			glm::vec2 texture_coords;
		};

		class ParticleRenderer
		{
		public :
			ParticleRenderer();
			void StartParticleRender();
			void RenderParticle(const Particle& particle, FPSCamera* camera);
			void EndParticleRender(FPSCamera* camera, GLClasses::Texture* texture_atlas);

		private :
			GLClasses::Shader m_ParticleShader;
			std::vector<ParticleVertex> m_ParticleVertices;

			GLClasses::VertexBuffer m_VBO;
			GLClasses::IndexBuffer m_IBO;
			GLClasses::VertexArray m_VAO;
		};

		class ParticleEmitter
		{
		public : 
			ParticleEmitter();
			void EmitParticlesAt(float lifetime, int num_particles, const glm::vec3& origin, 
				const glm::vec3& extent, const glm::vec3& vel, BlockType block);
			void OnUpdateAndRender(FPSCamera* camera, GLClasses::Texture* atlas);
			void CleanUpList();

		private :
			ParticleRenderer m_Renderer;
			std::vector<Particle> m_Particles;
		};
	}
}  