#include "Logger.h"

namespace Minecraft
{
	void Logger::LogToConsole(const std::string& text)
	{
		std::cout << "LOG : " << text << std::endl; 
	}

	void Logger::LogToConsole(const std::stringstream& s)
	{
		std::cout << "LOG : " << s.str() << std::endl;
	}

	void Logger::LogOpenGLInit(const char* renderer, const char* vendor, const char* version)
	{
		std::cout << "MINECRAFT CLONE V0.01 By Samuel Rasquinha (samuelrasquinha@gmail.com)" << std::endl << "\tRENDERER : "
			<< renderer << std::endl << "\tVENDOR : " << vendor << std::endl << "\tVERSION : " <<
			version << std::endl << "------------------------------------------------------------------------------------" << std::endl;
	}
}