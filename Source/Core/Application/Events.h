#pragma once

#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>

#include "EventStructs.h"

namespace Minecraft
{
	namespace EventSystem
	{
		void InitEventSystem(GLFWwindow* window, std::vector<Event>* event_queue);
	}
}