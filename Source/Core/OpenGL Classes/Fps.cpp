#include "Fps.h"

namespace GLClasses
{
	static const int max_fps = 65;
	static const bool print_fps = true;
	static long long int FPS_count = 0;

	void CalculateFrameRate(GLFWwindow* pWindow)
	{
		static const double expected_time = (double)(((double)1 / max_fps) * (double)1000.0);
		static double lastTime = 0;
		static float nbFrames = 0;
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		nbFrames++;
		if (delta >= 1.0) {

			double fps = double(nbFrames) / delta;
			std::stringstream ss;
			ss << "Opengl" << " " << "3.3" << " [" << fps << " FPS]";

			glfwSetWindowTitle(pWindow, ss.str().c_str());

			nbFrames = 0;
			lastTime = currentTime;
		}

		FPS_count += 1;
		Sleep(expected_time - (currentTime - lastTime));
	}

	long long int GetFPSCount()
	{
		return FPS_count;
	}

	void DisplayFrameRate(GLFWwindow* pWindow, const std::string& title)
	{
		static double lastTime = 0;
		static float nbFrames = 0;
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		nbFrames++;
		if (delta >= 1.0) { // If last cout was more than 1 sec ago
			//cout << 1000.0 / double(nbFrames) << endl;

			double fps = double(nbFrames) / delta;

			std::stringstream ss;
			ss << title << " [" << fps << " FPS]";

			glfwSetWindowTitle(pWindow, ss.str().c_str());

			nbFrames = 0;
			lastTime = currentTime;
		}

	}

	void CalculateFrameRateDeprecated()
	{
		static float framesPerSecond = 0.0f;
		static float fps;
		static double lastTime = 0.0f;
		double currentTime = glfwGetTime();
		++framesPerSecond;
		if (currentTime - lastTime > 1.0f)
		{
			lastTime = currentTime;
			fps = framesPerSecond;
			framesPerSecond = 0;
		}

		Sleep(((double)1 / max_fps * 1000) - (double)(currentTime - lastTime));
	}
}