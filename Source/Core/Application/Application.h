#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "../OpenGL Classes/Fps.h"

#include "Events.h"
#include "../World/World.h"
#include "../World/Skybox.h"
#include "../Block.h"
#include "../Chunk.h"

namespace Minecraft
{
	class Application
	{
	public : 
			
		GLFWwindow* m_Window;

		Application();
		void OnUpdate();

	private :

		void OnEvent(EventSystem::Event e);
		void PollEvents();

		World* m_World;

		std::vector<EventSystem::Event> m_EventQueue;
	};
}