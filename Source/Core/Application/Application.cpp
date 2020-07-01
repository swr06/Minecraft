#include "Application.h"

#include <cstdio>

#ifdef _DEBUG
#define NDEBUG
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
	Application::Application()
	{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(800, 600, "Minecraft V0.01", NULL, NULL);

        if (m_Window == NULL)
        {
            Logger::LogToConsole("Failed to create window!");
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_Window);

        // Turn on V-Sync
        glfwSwapInterval(1);

        glewInit();

        // Lock the cursor to the window
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_DEBUG_OUTPUT);

#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // disable if in release
#endif
		glDebugMessageCallback(gl_debug_callback, nullptr);

        // Turn on depth 
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        m_World = new World;

        EventSystem::InitEventSystem(m_Window, &m_EventQueue);
	}

	void Application::OnUpdate()
	{
        // Poll the events
        PollEvents();

        // Update the world
        m_World->OnUpdate(m_Window);

        // Start rendering
        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Clear the depth and color bit buffer
        glClearColor(0.44f, 0.78f, 0.88f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        m_World->RenderWorld();

        GLClasses::DisplayFrameRate(m_Window, "Minecraft Clone V0.01 ");
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
	}

	void Application::OnEvent(EventSystem::Event e)
	{
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
}
