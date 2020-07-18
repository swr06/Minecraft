#pragma once

#include <string>
#include <iostream>
#include <sstream>

namespace Minecraft
{
	namespace Logger
	{
		void LogToConsole(const std::string& text);
		void LogToConsole(const std::stringstream& s);

		// Only called once in the program (ie : When the program is started)
		void LogOpenGLInit(const char* renderer, const char* vendor, const char* version);
	}
}