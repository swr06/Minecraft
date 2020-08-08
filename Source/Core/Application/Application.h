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

// Include GUI Library
#include "../UI/GUI.h"

#include "../Clouds/Cloud.h"

namespace Minecraft
{
	enum GameState : uint8_t
	{
		MenuState = 0,
		WorldSelectState,
		WorldCreateState,
		PlayingState,
		PauseState,
		HelpState
	};

	class Application
	{
	public : 
		
		GLFWwindow* m_Window;

		Application();
		~Application();

		void OnUpdate();
		inline World* GetWorld() { return m_World; }

		GameState GetState() { return m_GameState; }
	private :

		void OnImGuiRender();
		void OnEvent(EventSystem::Event e);
		void PollEvents();

		World* m_World;

		std::vector<EventSystem::Event> m_EventQueue;
		bool m_CursorLocked;
		GameState m_GameState;
		ImFont* m_Font;
	};

	extern Application MinecraftApplication;

	Application* GetMinecraftApp();
}