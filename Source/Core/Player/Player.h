#pragma once

#include <iostream>
#include <string>

#include "../FpsCamera.h"
#include "../Block.h"

namespace Minecraft
{
	class World;

	class Player
	{
	public : 

		void OnUpdate();
		void OnEvent();

		FPSCamera p_Camera;
		glm::vec3 p_Position;
		World* p_World;
	};
}