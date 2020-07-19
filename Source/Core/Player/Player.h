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

	class Player
	{
	public : 

		Player() : p_Camera(70.0f, 800 / 600, 0.1, 100.0f)
		{
			p_PlayerAABB.x = 0;
			p_PlayerAABB.y = 0;
			p_PlayerAABB.z = 0;
			p_PlayerAABB.width = 16;
			p_PlayerAABB.height = 32;
			p_PlayerAABB.depth = 16;
		}

		void OnUpdate();
		void OnEvent(EventSystem::Event e);

		FPSCamera p_Camera;
		glm::vec3 p_Position;
		World* p_World;
		AABB p_PlayerAABB;
	};
}