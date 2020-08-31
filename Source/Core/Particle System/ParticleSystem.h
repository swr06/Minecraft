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

namespace Minecraft
{
	namespace ParticleSystem
	{
		class ParticleRenderer
		{
		public :
			ParticleRenderer();
			void RenderParticle(const Particle& particle);

		private :
			GLClasses::Shader m_ParticleShader;
		};
	}
}