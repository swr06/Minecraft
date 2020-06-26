#include "Logger.h"

namespace Minecraft
{
	void Logger::LogToConsole(const std::string& text)
	{
		std::cout << "LOG : " << text << std::endl; 
	}
}