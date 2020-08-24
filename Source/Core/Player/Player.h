#pragma once

#include <iostream>
#include <string>

#include "../FpsCamera.h"
#include "../Block.h"
#include "../Application/Events.h"

namespace Minecraft
{
	class World;
	
	Block* GetWorldBlock(const glm::vec3& block_pos);  

	class Player
	{
	public : 

		Player(float wx, float wy) : p_Camera(70.0f, wx / wy, 0.1, 500)
		{

		}

		void OnUpdate(GLFWwindow* window);
		void OnEvent(EventSystem::Event e);
		bool TestBlockCollision(const glm::vec3& position);

		FPSCamera p_Camera;
		glm::vec3 p_Position;
		World* p_World;
		std::uint8_t p_CurrentHeldBlock = 0;
		bool p_IsColliding = false;
		bool p_FreeFly = false;

	private : 

		bool m_IsJumping = false;
		bool m_IsFalling = false;
	};
}