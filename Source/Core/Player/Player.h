#pragma once

#include <iostream>
#include <string>

#include "../FpsCamera.h"
#include "../Block.h"
#include "../Application/Events.h"
#include "../Physics/AABB.h"

namespace Minecraft
{
	class World;
	
	Block* GetWorldBlock(const glm::vec3& block_pos);

	class Player
	{
	public : 

		Player() : p_Camera(70.0f, 800 / 600, 0.1, 100.0f), p_PlayerAABB(glm::vec3(-0.25f, -1.5f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f))
		{

		}

		void OnUpdate(GLFWwindow* window);
		void OnEvent(EventSystem::Event e);

		FPSCamera p_Camera;
		glm::vec3 p_Position;
		World* p_World;
		AABB p_PlayerAABB;

	private : 
		bool TestBlockCollision(const glm::vec3& position);
	};
}