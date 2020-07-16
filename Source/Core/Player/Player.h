#pragma once

#include <iostream>
#include <string>

#include "../FpsCamera.h"
#include "../Block.h"
#include "../Application/Events.h"

namespace Minecraft
{
	class World;

	class Player
	{
	public : 

		Player() : p_Camera(70.0f, 800/600, 0.1, 100.0f)
		{

		}

		void OnUpdate();
		void OnEvent(EventSystem::Event e);

		FPSCamera p_Camera;
		glm::vec3 p_Position;
		World* p_World;
	};
}