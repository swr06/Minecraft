#pragma once

#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw3.h>

namespace Minecraft
{
	Block* GetWorldBlock(const glm::vec3& block_pos);

	namespace ParticleSystem
	{

		constexpr float gravity = 1;

		enum class ParticleDirection
		{
			left = 0,
			right
		};

		class Particle
		{
		public :

			Particle(const glm::vec3& position, const glm::vec3& velocity, const float lifetime, const float scale, ParticleDirection dir)
			{
				p_ElapsedTime = 0.0f;
				p_Rotation = 0.0f;
				p_Position = position;
				p_Velocity = velocity;
				p_Lifetime = lifetime / 2;
				p_Scale = scale;
				p_IsAlive = true;
				m_Dir = dir;
			}

			void OnUpdate()
			{
				glm::vec3 pos_before = p_Position;

				// Update delta every frame
				float delta = 0.1f;

				// Update the particle
				p_Velocity.y -= gravity * delta;
				glm::vec3 change = p_Velocity;
				change *= delta;

				p_Position.y += change.y;

				float multiplier = 1.0f;

				if (GetWorldBlock(glm::floor(p_Position))->Collidable())
				{
					p_Position = pos_before;
					float t = 1.0f - (1.0f / (p_Lifetime - p_ElapsedTime));
					multiplier = (1.0 - t) * 0.1f + t * 0.015f;
				}

				if (m_Dir == ParticleDirection::right)
				{
					p_Position.x += change.x * multiplier;
					p_Position.z += change.z * multiplier;
				}

				else
				{
					p_Position.x -= change.x * multiplier;
					p_Position.z -= change.z * multiplier;
				}

				p_ElapsedTime += delta;
				p_IsAlive = IsAlive();
			}

			const glm::vec3& GetPosition() const { return p_Position; }
			const glm::vec3& GetVelocity() const { return p_Velocity; }
			float GetLifetime() const { return p_Lifetime; }
			float GetElapsedTime() const { return p_ElapsedTime; }
			const float GetScale() const { return p_Scale; }
			bool IsAlive() const { return p_Lifetime > p_ElapsedTime; }
			bool ISDead() const { return p_Lifetime < p_ElapsedTime; }

			glm::vec3 p_Position;
			glm::vec3 p_Velocity;
			float p_Lifetime;	
			float p_ElapsedTime;
			float p_Rotation;
			float p_Scale;
			bool p_IsAlive;

			BlockType p_BlockType;

		private :
			ParticleDirection m_Dir;
		};
	}
}