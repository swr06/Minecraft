#pragma once

#include <iostream>
#include <string>

#include "../FpsCamera.h"
#include "../Block.h"
#include "../World/World.h"

namespace Minecraft
{
	class Player
	{
	public : 
		FPSCamera p_Camera;
		glm::vec3 p_Position;
		World* p_World;
	};
}