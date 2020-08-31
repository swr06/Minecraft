#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h>

// Include imgui
//#include "../Dependencies/imgui/imgui.h"
//#include "../Dependencies/imgui/imgui_impl_glfw.h"
//#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
#include "PlatformSpecific.h"

namespace Minecraft
{
	enum GameState : uint8_t
	{
		MenuState = 0,
		WorldSelectState,
		WorldCreateState,
		PlayingState,
		PauseState,
		HelpState,
		SettingsState
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
		bool m_VSync = true;
		bool m_ShowDebugInfo = false;
		GameState m_GameState;
		ImFont* m_Font;
		Texture m_LogoTexture;
		Texture m_BlurMenuBackground;
		Renderer2D* m_Renderer2D;
		OrthographicCamera m_OrthagonalCamera;
		PlatformSpecific::ProcessDebugInfo m_ProcDebugInfo;
	};

	extern Application MinecraftApplication;

	Application* GetMinecraftApp();
}