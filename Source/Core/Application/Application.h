#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include imgui
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"

#include <iostream>

#include "../OpenGL Classes/Fps.h"

#include "Events.h"
#include "../File Handling/WorldFileHandler.h"
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
		~Application();

		void OnUpdate();
		inline World* GetWorld() { return m_World; }

	private :

		void OnImGuiRender();
		void OnEvent(EventSystem::Event e);
		void PollEvents();

		World* m_World;

		std::vector<EventSystem::Event> m_EventQueue;
		bool m_CursorLocked;
	};

	extern Application MinecraftApplication;

	Application* GetMinecraftApp();
}