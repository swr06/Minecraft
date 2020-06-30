#include "Fps.h"

namespace GLClasses
{


	void DisplayFrameRate(GLFWwindow* pWindow, const std::string& title)
	{
		static double lastTime = 0;
		static float nbFrames = 0;
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		nbFrames++;
		if (delta >= 1.0)
		{
			double fps = double(nbFrames) / delta;

			std::stringstream ss;
			ss << title << " [" << fps << " FPS]";

			glfwSetWindowTitle(pWindow, ss.str().c_str());

			nbFrames = 0;
			lastTime = currentTime;
		}
	}
}