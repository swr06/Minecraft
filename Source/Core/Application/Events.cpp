#include "Events.h"

namespace Minecraft
{
	namespace EventSystem
	{
		static std::vector<Event>* EventQueue;
		
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void MouseCallback(GLFWwindow* window, int button, int action, int mods);
		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		void InitEventSystem(GLFWwindow* window, std::vector<Event>* event_queue)
		{
			// Set the event queue
			EventQueue = event_queue;

			// Set all the event callbacks

			glfwSetKeyCallback(window, KeyCallback);
			glfwSetMouseButtonCallback(window, MouseCallback);
			glfwSetScrollCallback(window, ScrollCallback);
			glfwSetCursorPosCallback(window, CursorPosCallback);
			glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		}

		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Event e;

			switch (action)
			{
				case GLFW_PRESS : 
					e.type = EventTypes::KeyPress;
					break;

				case GLFW_RELEASE : 
					e.type = EventTypes::KeyRelease;
					break;

				default : 
					e.type = EventTypes::Undefined;
					break;
			}

			e.key = key;
			e.mods = mods;

			EventQueue->push_back(e);
		}

		void MouseCallback(GLFWwindow* window, int button, int action, int mods)
		{
			Event e;

			switch (action)
			{
				case GLFW_PRESS:
					e.type = EventTypes::MousePress;
					break;

				case GLFW_RELEASE : 
					e.type = EventTypes::MouseRelease;
					break;

				default : 
					e.type = EventTypes::Undefined;
					break;
			}

			e.button = button;
			e.mods = mods;

			EventQueue->push_back(e);
		}

		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			Event e;

			e.type = EventTypes::MouseScroll;
			e.msx = xoffset;
			e.msy = yoffset;
			EventQueue->push_back(e);
		}

		void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
		{
			Event e;

			e.type = EventTypes::MouseMove;
			e.mx = xpos;
			e.my = ypos;

			EventQueue->push_back(e);
		}

		void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			Event e;

			e.type = EventTypes::WindowResize;
			e.wx = width;
			e.wy = height;

			EventQueue->push_back(e);
		}
	}
}