#include "Application.h"

#include <cstdio>

#ifdef _DEBUG
#define NDEBUG
#endif

// Needs OpenGL 4+
#if GL_VERSION_MAJOR > 3
#define SHOULD_DEBUG_GL
#endif

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const char* message, const void*)
{
	const char* sev = "";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: sev = "\e[91m"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: sev = "\e[93m"; break;
	case GL_DEBUG_SEVERITY_LOW: sev = "\e[92m"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "\e[34m"; break;
	}

	const char* src = "?";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API: src = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src = "window system"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "shader compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: src = "third party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: src = "app"; break;
	case GL_DEBUG_SOURCE_OTHER: src = "other"; break;
	}

	const char* type_str = "?";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR: type_str = "error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "deprecated behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "undefined behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY: type_str = "portability"; break;
	case GL_DEBUG_TYPE_MARKER: type_str = "marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: type_str = "push group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: type_str = "pop group"; break;
	case GL_DEBUG_TYPE_OTHER: type_str = "other"; break;
	}

	fprintf(stderr, "debug:%s type: %s, source: %s, message: \"%.*s\"\e[0m\n", sev, type_str, src, length, message);
}


namespace Minecraft
{
	static const constexpr bool ShouldInitializeImgui = true;
	constexpr float default_player_speed = 0.05f;
	constexpr float default_player_sensitivity = 0.25f;
	float ex_PlayerSpeed = default_player_speed;
	float ex_PlayerSensitivity = default_player_sensitivity;

	Application MinecraftApplication;

	static void glfwErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "GLFW ERROR!   %d: %s\n", error, description);
	}

	bool FilenameIsValid(const std::string& filepath)
	{
		FILE* file;

		file = fopen(filepath.c_str(), "w+");
		
		if (!file)
		{
			return false;
		}

		else
		{
			fclose(file);
			std::filesystem::remove(filepath);
			return true;
		}
	}

	Application::Application() : m_GameState(GameState::MenuState), m_OrthagonalCamera(0.0f, (float)DEFAULT_WINDOW_X, 0.0f, (float)DEFAULT_WINDOW_Y)
	{
		const char* glsl_version = static_cast<const char*>("#version 130");

		glfwSetErrorCallback(glfwErrorCallback);

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y, "A Tiny Minecraft Clone V0.01 By Samuel Rasquinha", NULL, NULL);

		if (m_Window == NULL)
		{
			Logger::LogToConsole("Failed to create window!");
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);

		// Turn on V-Sync
		glfwSwapInterval(1);

		glewInit();

		char* renderer = (char*)glGetString(GL_RENDERER);
		char* vendor = (char*)glGetString(GL_VENDOR);
		char* version = (char*)glGetString(GL_VERSION);

		Logger::LogOpenGLInit(renderer, vendor, version);

		// Lock the cursor to the window
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_DEBUG_OUTPUT);

		// Enable synchronus debugging if the opengl version is 4.0 or 4.0+
#ifdef SHOULD_DEBUG_GL
#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // disable if in release
#endif
		glDebugMessageCallback(gl_debug_callback, nullptr);
#endif

		EventSystem::InitEventSystem(m_Window, &m_EventQueue);

		// Setting up imgui context
		if (ShouldInitializeImgui)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			//m_Font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/pixeboy.ttf", 24);
			m_Font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Pixellari.ttf", 24);
			io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
			io.IniFilename = nullptr;
		}

		// Turn on depth 
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_CursorLocked = true;

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		GUI::InitUISystem(m_Window);

		// Resize window to the maximized view
		glfwMaximizeWindow(m_Window);

		m_LogoTexture.CreateTexture("Resources/Branding/logo.png", false);
		m_BlurMenuBackground.CreateTexture("Resources/Branding/menu_blur.png", false);
		m_Renderer2D = new Renderer2D;
	}

	Application::~Application()
	{
		if (ShouldInitializeImgui)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		GUI::CloseUIContext();
		//Clouds::DestroyClouds();
		glfwDestroyWindow(m_Window);

		delete m_Renderer2D;
		delete m_World;
	}

	void Application::OnUpdate()
	{
		if (ShouldInitializeImgui)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		// Poll the events
		PollEvents();

		// Enable depth testing and blending
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Clear the depth and color bit buffer

		glClearColor(0.44f, 0.78f, 0.88f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		if (m_GameState == GameState::PlayingState)
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Update the world
			m_World->OnUpdate(m_Window);

			// Render the world
			m_World->RenderWorld();
		}

		if (ShouldInitializeImgui)
		{
			OnImGuiRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		GUI::RenderUI(glfwGetTime(), 0);

		// Render imgui and swap the buffers after rendering ui components, world etc..
		GLClasses::DisplayFrameRate(m_Window, "A Tiny Minecraft Clone V0.01 By Samuel Rasquinha");
		glfwSwapBuffers(m_Window);
		glfwPollEvents();

		// Obtain the debug info every second
		
		if (std::remainderf(glfwGetTime(), 2.0f) <= 0.0f)
		{
			PlatformSpecific::GetProcDebugInfo(m_ProcDebugInfo);
		}
	}

	void Application::OnImGuiRender()
	{
		static bool first_run = true;
		int w, h;
		glfwGetFramebufferSize(m_Window, &w, &h);

		// For the world create menu
		static char input[64];
		static int seed = 0;
		static int world_type = 0;

		std::vector<std::string> Saves;

		if (m_GameState == GameState::PlayingState && m_World)
		{
			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoScrollbar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(w - 10, h - 10), ImGuiCond_Always);

			if (ImGui::Begin("Debug Text", &open, window_flags))
			{
				stringstream ss;
				BlockType current_block = static_cast<BlockType>(m_World->p_Player->p_CurrentHeldBlock);
				const glm::vec3& pos = m_World->p_Player->p_Position;

				ss << "VSync : " << m_VSync << "\n";
				ss << "Current held block : " << BlockDatabase::GetBlockName(current_block).c_str() << "\n";
				ss << "Player Position =  X : " << pos.x << " | Y : " << pos.y << " | Z : " << pos.z << "\n"; 
				ss << "Player is colliding : " << m_World->p_Player->p_IsColliding << "\n";
				ss << "Freefly : " << m_World->p_Player->p_FreeFly << "\n";
				ss << "Chunk amount : " << m_World->m_ChunkCount << "\n";
				ImGui::Text(ss.str().c_str());
				
				if (m_ShowDebugInfo)
				{
					std::stringstream debug_ss;

					debug_ss << "This menu is experimental! \n";
					debug_ss << "Total CPU Used : " << m_ProcDebugInfo.cpu_usage << "\n";
					debug_ss << "Total Memory : " << m_ProcDebugInfo.total_mem << "  /  " << m_ProcDebugInfo.total_mem_used << "\n";
					debug_ss << "Total Virtual Memory : " << m_ProcDebugInfo.total_vm << "  /  " << m_ProcDebugInfo.total_vm_used << "\n";
					debug_ss << "CPU Usage : " << m_ProcDebugInfo.cpu_usage << "\n";
					ImGui::Text(debug_ss.str().c_str());
				}

				ImGui::End();
			}
		}

		if (first_run)
		{
			memset(&input, '\0', 64);

			first_run = false;
		}
		
		if (m_GameState == GameState::MenuState)
		{
			float logo_x, logo_y;
			
			logo_x = w * 0.10f;
			logo_y = h - m_LogoTexture.GetHeight();
			logo_y -= h * 0.15f;

			float logo_width = w - (w * 0.10);
			float logo_height = h - (h * 0.15);

			m_Renderer2D->RenderQuad(glm::vec2(0, 0), &m_BlurMenuBackground, &m_OrthagonalCamera, w, h);
			m_Renderer2D->RenderQuad(glm::vec2(logo_x, logo_y), &m_LogoTexture, &m_OrthagonalCamera, logo_width, logo_height);
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoScrollbar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2((w/2) - 220/2, (h/2) - 70/2), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Always);

			if (ImGui::Begin("Menu", &open, window_flags))
			{
				ImGui::PushFont(m_Font);

				if (ImGui::Button("PLAY!", ImVec2(200, 48)))
				{
					m_GameState = GameState::WorldSelectState;
				}

				ImGui::NewLine();

				if (ImGui::Button("HELP AND ABOUT", ImVec2(200, 48)))
				{
					m_GameState = GameState::HelpState;
				}

				ImGui::NewLine();

				if (ImGui::Button("SETTINGS", ImVec2(200, 48)))
				{
					m_GameState = GameState::SettingsState;
				}

				ImGui::NewLine();

				if (ImGui::Button("EXIT!", ImVec2(200, 48)))
				{
					glfwSetWindowShouldClose(m_Window, true);
				}

				ImGui::PopFont();
				ImGui::End();
			}
		}

		static GameState prev_settings_state;

		if (m_GameState != GameState::SettingsState)
		{
			prev_settings_state = m_GameState;
		}


		if (m_GameState == GameState::SettingsState)
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoScrollbar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			window_flags |= ImGuiWindowFlags_NoBackground;

			//ImGui::SetNextWindowPos(ImVec2((w / 2) - 250.0f, (h / 2) - 100.0f), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(50,50), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);

			if (ImGui::Begin("Settings Menu", &open, window_flags))
			{
				m_Renderer2D->RenderQuad(glm::vec2(0, 0), &m_BlurMenuBackground, &m_OrthagonalCamera, w, h);
				ImGui::PushFont(m_Font);

				ImGui::Text("Settings! \n\n");

				ImGui::SliderFloat("Player Speed", &ex_PlayerSpeed, 0.01, 0.1);
				ImGui::SliderFloat("Sensitivity", &ex_PlayerSensitivity, 0.01, 1.5f);

				ImGui::NewLine();
				ImGui::NewLine();

				if (ImGui::Button("Ok"))
				{
					m_GameState = prev_settings_state;
				}

				if (ImGui::Button("Reset"))
				{
					ex_PlayerSpeed = default_player_speed;
					ex_PlayerSensitivity = default_player_sensitivity;
				}

				ImGui::NewLine();

				ImGui::PopFont();
				ImGui::End();
			}
		}

		else if (m_GameState == GameState::PauseState)
		{
			m_Renderer2D->RenderQuad(glm::vec2(0, 0), &m_BlurMenuBackground, &m_OrthagonalCamera, w, h);
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoScrollbar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2((w / 2) - 220 / 2, (h / 2) - 70 / 2), ImGuiCond_Always);

			if (ImGui::Begin("Pause Menu.", &open, window_flags))
			{
				ImGui::PushFont(m_Font);

				if (ImGui::Button("RESUME", ImVec2(200, 48)))
				{
					m_GameState = GameState::PlayingState;
				}

				ImGui::NewLine();

				if (ImGui::Button("SETTINGS", ImVec2(200, 48)))
				{
					m_GameState = GameState::SettingsState;
				}

				ImGui::NewLine();

				if (ImGui::Button("QUIT", ImVec2(200, 48)))
				{
					WorldFileHandler::SaveWorld(m_World->GetName(), m_World);
					m_GameState = GameState::MenuState;

					delete m_World;
					m_World = nullptr;
				}

				ImGui::PopFont();
				ImGui::End();
			}
		}

		else if (m_GameState == GameState::WorldSelectState)
		{
			m_Renderer2D->RenderQuad(glm::vec2(0, 0), &m_BlurMenuBackground, &m_OrthagonalCamera, w, h);

			if (std::filesystem::exists("Saves/"))
			{
				for (auto entry : std::filesystem::directory_iterator("Saves/"))
				{
					Saves.push_back(entry.path().filename().string());
				}
			}

			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(w - 20, h - 20), ImGuiCond_Always);

			if (ImGui::Begin("Menu", &open, window_flags))
			{
				ImGui::PushFont(m_Font);

				if (ImGui::Button("Create new world.."))
				{
					m_GameState = GameState::WorldCreateState;
				}

				ImGui::SameLine();
				ImGui::Text("                     ");
				ImGui::SameLine();

				if (ImGui::Button("BACK"))
				{
					m_GameState = GameState::MenuState;
				}

				ImGui::Separator();
				ImGui::NewLine();

				for (int i = 0; i < Saves.size(); i++)
				{
					if (ImGui::Button(Saves.at(i).c_str()))
					{
						m_World = WorldFileHandler::LoadWorld(Saves.at(i));
						m_GameState = GameState::PlayingState;
						break;
					}

					ImGui::Separator();
				}

				ImGui::NewLine();

				ImGui::PopFont();
				ImGui::End();
			}
		}

		else if (m_GameState == GameState::WorldCreateState)
		{
			m_Renderer2D->RenderQuad(glm::vec2(0, 0), &m_BlurMenuBackground, &m_OrthagonalCamera, w, h);

			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(w - 20, h - 20), ImGuiCond_Always);

			if (ImGui::Begin("World Create", &open, window_flags))
			{
				ImGui::PushFont(m_Font);

				if (ImGui::Button("BACK"))
				{
					m_GameState = GameState::WorldSelectState;
				}

				ImGui::Text("\n");
				ImGui::Separator();
				ImGui::Text("\n");
				ImGui::Text("Create your new world!");
				ImGui::Text("\n");
				ImGui::InputText("World Name", input, 63);
				ImGui::InputInt("Seed", &seed);
				ImGui::Text("\n\n");
				ImGui::RadioButton("Normal", &world_type, (int)WorldGenerationType::Generation_Normal);
				ImGui::RadioButton("Normal without caves (Faster)", &world_type, (int)WorldGenerationType::Generation_Normal_withoutcaves);
				ImGui::RadioButton("Flat", &world_type, (int)WorldGenerationType::Generation_Flat);
				ImGui::RadioButton("Flat without Structures", &world_type, (int)WorldGenerationType::Generation_FlatWithoutStructures);

				ImGui::Text("\n\n");

				if (ImGui::Button("Create!", ImVec2(200,200)))
				{
					bool isValid = FilenameIsValid(input);

					if (isValid)
					{
						m_World = new World(seed, glm::vec2(w, h), input, static_cast<WorldGenerationType>(world_type));
						m_GameState = GameState::PlayingState;
						memset(input, '\0', 64);
					}

					else
					{
						Logger::LogToConsole("INVALID WORLD NAME GIVEN!");
					}
				}

				ImGui::PopFont();
				ImGui::End();
			}
		}

		else if (m_GameState == GameState::HelpState)
		{
			m_Renderer2D->RenderQuad(glm::vec2(0, 0), &m_BlurMenuBackground, &m_OrthagonalCamera, w, h);
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bool open = true;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			//window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);

			if (ImGui::Begin("Help Menu", 0, window_flags))
			{
				if (ImGui::Button("BACK"))
				{
					m_GameState = GameState::MenuState;
				}

				ImGui::Separator();
				ImGui::Text("\n");
				ImGui::Text("A Tiny Minecraft Clone");
				ImGui::Text("By Samuel Rasquinha (samuelrasquinha@gmail.com)");
				ImGui::Text("Discord : swr#1899");
				ImGui::Text("If you like this project. Please consider starring it on GitHub");
				ImGui::Text("All art and resources are not mine. Credits go to their respective owners");
				ImGui::Text("\n\n");
				ImGui::Text("Instructions : ");
				ImGui::Text("TOGGLE DEBUG MENU : F3");
				ImGui::Text("MOVEMENT - W S A D");
				ImGui::Text("FLY - SPACE/LEFT SHIFT");
				ImGui::Text("BLOCK EDITING - LEFT/RIGHT MOUSE BUTTONS");
				ImGui::Text("CHANGE CURRENT BLOCK - (Q) or (E)");

				ImGui::End();
			}
		}
	}

	void Application::OnEvent(EventSystem::Event e)
	{
		switch (e.type)
		{
		case EventSystem::EventTypes::MousePress:
		{
			GUI::MouseButtonCallback(e.button, GLFW_PRESS, e.mods);
			break;
		}

		case EventSystem::EventTypes::MouseRelease:
		{
			GUI::MouseButtonCallback(e.button, GLFW_RELEASE, e.mods);
			break;
		}

		case EventSystem::EventTypes::WindowResize:
		{
			glViewport(0, 0, e.wx, e.wy);
			m_OrthagonalCamera.SetProjection(0.0f, e.wx, 0.0f, e.wy);
			break;
		}

		case EventSystem::EventTypes::KeyPress:
		{
			if (e.key == GLFW_KEY_ESCAPE && m_GameState == GameState::PlayingState)
			{
				m_GameState = GameState::PauseState;
			}

			else if (e.key == GLFW_KEY_F3 && m_GameState == GameState::PlayingState)
			{
				m_ShowDebugInfo = !m_ShowDebugInfo;
			}

			else if (e.key == GLFW_KEY_ESCAPE && m_GameState == GameState::PauseState)
			{
				m_GameState = GameState::PlayingState;
			}

			else if (e.key == GLFW_KEY_V)
			{
				m_VSync = !m_VSync;
				glfwSwapInterval(m_VSync);
			}

			break;
		}
		}

		if (m_World)
		{
			m_World->OnEvent(e);
		}
	}

	void Application::PollEvents()
	{
		for (int i = 0; i < m_EventQueue.size(); i++)
		{
			OnEvent(m_EventQueue[i]);
		}

		m_EventQueue.erase(m_EventQueue.begin(), m_EventQueue.end());
	}

	Application* GetMinecraftApp()
	{
		return &MinecraftApplication;
	}
}
