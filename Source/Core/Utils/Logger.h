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
	}
}