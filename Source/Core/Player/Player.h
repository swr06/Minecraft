#pragma once

#include <iostream>
#include <string>

#include "../FpsCamera.h"
#include "../Block.h"
#include "../Utils/Ray.h"
#include "../Application/Events.h"

namespace Minecraft
{
	class World;

	class Player
	{
	public : 

		Player() : p_Camera(70.0f, 800/600, 0.1, 100.0f), p_RayCast(&p_Camera)
		{

		}

		void OnUpdate();
		void OnEvent(EventSystem::Event e);

		FPSCamera p_Camera;
		glm::vec3 p_Position;
		RayCast p_RayCast;
		World* p_World;
	};
}