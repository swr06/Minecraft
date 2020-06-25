#include "Events.h"

namespace Minecraft
{
	namespace EventSystem
	{
		static std::vector<Event>* EventQueue;
		
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void WindowSizeCallback(GLFWwindow* window, int width, int height);
		void MouseCallback(GLFWwindow* window, int button, int action, int mods);
		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		void InitEventSystem(GLFWwindow* window, std::vector<Event>* event_queue)
		{
			// Set all the event callbacks

			glfwSetKeyCallback(window, KeyCallback);
			glfwSetWindowSizeCallback(window, WindowSizeCallback);
			glfwSetMouseButtonCallback(window, MouseCallback);
			glfwSetScrollCallback(window, ScrollCallback);
			glfwSetCursorPosCallback(window, CursorPosCallback);
			glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		}

		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{

		}

		void WindowSizeCallback(GLFWwindow* window, int width, int height)
		{

		}

		void MouseCallback(GLFWwindow* window, int button, int action, int mods)
		{

		}

		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{

		}

		void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
		{

		}

		void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
		{

		}
	}
}