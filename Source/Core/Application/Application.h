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
		~Application()
		{
			delete m_World;
		}

		void OnUpdate();
		inline World* GetWorld() { return m_World; }

	private :

		void OnEvent(EventSystem::Event e);
		void PollEvents();

		World* m_World;

		std::vector<EventSystem::Event> m_EventQueue;
	};

	extern Application MinecraftApplication;

	Application* GetMinecraftApp();
}