#pragma once

#include <iostream>

#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/IndexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/Texture.h"
#include "../OpenGL Classes/GLDebug.h"
#include "../FpsCamera.h"

#include "../Particle System/Particle.h"
#include "../Utils/Random.h"

namespace Minecraft
{
	namespace ParticleSystem
	{
		class ParticleRenderer
		{
		public :
			ParticleRenderer();
			void RenderParticle(const Particle& particle, FPSCamera* camera);

		private :
			GLClasses::Shader m_ParticleShader;
		};

		class ParticleEmitter
		{
		public : 
			ParticleEmitter();
			void EmitParticlesAt(float lifetime, int num_particles, const glm::vec3& origin, const glm::vec2& extent, const glm::vec3& vel);
			void OnUpdateAndRender(FPSCamera* camera);
			void CleanUpList();

		private :
			std::vector<Particle> m_Particles;
			ParticleRenderer m_Renderer;
		};
	}
}