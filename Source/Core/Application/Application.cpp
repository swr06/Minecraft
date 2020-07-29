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
		const char *message, const void *) 
{
	const char *sev = "";

	switch (severity) 
	{
		case GL_DEBUG_SEVERITY_HIGH: sev = "\e[91m"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: sev = "\e[93m"; break;
		case GL_DEBUG_SEVERITY_LOW: sev = "\e[92m"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "\e[34m"; break;
	}

	const char *src = "?";

	switch (source) 
	{
		case GL_DEBUG_SOURCE_API: src = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src = "window system"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "shader compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: src = "third party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: src = "app"; break;
		case GL_DEBUG_SOURCE_OTHER: src = "other"; break;
	}

	const char *type_str = "?";

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
	static const constexpr bool ShouldInitializeImgui = false;
	static const constexpr bool ShouldCull = true;

	Application MinecraftApplication;

	static void glfwErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "GLFW ERROR!   %d: %s\n", error, description);
	}

	Application::Application()
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

		// Setting up imgui context
		if (ShouldInitializeImgui)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}

        // Turn on depth 
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_CursorLocked = true;

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		bool load_world = false;

		std::cout << "\nDo you want to load the saved world ? (1/0) : ";
		std::cin >> load_world;
		std::cout << "\n";

		if (load_world)
		{
			m_World = WorldFileHandler::LoadWorld("WORLD_1");

			if (!m_World)
			{
				Logger::LogToConsole("Failed to load the world! reverting to generating a new world.");
				m_World = new World;
			}
		}

		else
		{
			m_World = new World;
		}

        EventSystem::InitEventSystem(m_Window, &m_EventQueue);

		// Resize window to the maximized view
		glfwMaximizeWindow(m_Window);
	}

	Application::~Application()
	{
		if (ShouldInitializeImgui)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		bool save_world = false;
		std::cout << "\n\n\n" << "Do you want to save your world ? (0/1) : "; 
		std::cin >> save_world;

		if (save_world)
		{
			WorldFileHandler::SaveWorld("WORLD_1", m_World);
		}

		glfwDestroyWindow(m_Window);

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

        // Update the world
        m_World->OnUpdate(m_Window);
		
		// Enable depth testing and blending
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Clear the depth and color bit buffer

        glClearColor(0.44f, 0.78f, 0.88f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Render the world
        m_World->RenderWorld();

		if (ShouldInitializeImgui)
		{
			OnImGuiRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		// Render imgui and swap the buffers after rendering ui components, world etc..
		GLClasses::DisplayFrameRate(m_Window, "A Tiny Minecraft Clone V0.01 By Samuel Rasquinha");
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
	}

	void Application::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
	}

	void Application::OnEvent(EventSystem::Event e)
	{
		switch (e.type)
		{
			case EventSystem::EventTypes::WindowResize : 
			{
				glViewport(0, 0, e.wx, e.wy);

				break;
			}

			case EventSystem::EventTypes::KeyPress :
			{
				if (e.key == GLFW_KEY_F1)
				{
					glfwSetWindowShouldClose(m_Window, true);
				}

				else if (e.key == GLFW_KEY_ESCAPE)
				{
					// If the cursor is locked, release it. else lock it back
					if (m_CursorLocked == true)
					{
						glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						m_CursorLocked = false;
					}

					else if (m_CursorLocked == false)
					{
						glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						m_CursorLocked = true;
					}
				}
			}
		}

        m_World->OnEvent(e);
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
