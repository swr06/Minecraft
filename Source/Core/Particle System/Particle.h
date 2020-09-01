#pragma once

#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw3.h>

namespace Minecraft
{
	namespace ParticleSystem
	{
		constexpr float gravity = 1;

		class Particle
		{
		public :

			Particle(const glm::vec3& position, const glm::vec3& velocity, const float lifetime, const float rot, const float scale)
			{
				p_Position = position;
				p_Velocity = velocity;
				p_Lifetime = lifetime;
				p_Rotation = rot;
				p_Scale = scale;

			}

			bool OnUpdate()
			{
				// Update delta every frame
				float delta = 0.0025f;

				// Update the particle
				//p_Velocity.y -= gravity * delta; // THIS IS TO MAKE IT FALL OVER TIME!
				glm::vec3 change = p_Velocity;
				change *= delta;

				p_Position += change;
				
				p_ElapsedTime += delta;
				return IsAlive();
			}

			const glm::vec3& GetPosition() const { return p_Position; }
			const glm::vec3& GetVelocity() const { return p_Velocity; }
			float GetLifetime() const { return p_Lifetime; }
			float GetElapsedTime() const { return p_ElapsedTime; }
			const float GetRot() const { return p_Rotation; }
			const float GetScale() const { return p_Scale; }
			bool IsAlive() const { return p_Lifetime > p_ElapsedTime; }
			bool ISDead() const { return p_Lifetime < p_ElapsedTime; }

			glm::vec3 p_Position;
			glm::vec3 p_Velocity;
			float p_Lifetime;	
			float p_ElapsedTime;
			float p_Rotation;
			float p_Scale;

			
		};
	}
}