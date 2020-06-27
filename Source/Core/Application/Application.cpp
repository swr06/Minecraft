#include "Application.h"

namespace Minecraft
{
	Application::Application()
	{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
        
        // Clear the depth and color bit buffer
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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