#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include "Logger.h"

namespace Minecraft
{
	class Timer
	{
	public : 

		Timer(const std::string& output_text) : m_Output(output_text)
		{
			std::stringstream out;

			m_StartTime = std::chrono::system_clock::now();

			out << output_text << "  | TIMER STARTED";
			Logger::LogToConsole(out.str());
		}
		
		~Timer()
		{
			End();
		}

		void Start()
		{
			std::stringstream out;

			m_StartTime = std::chrono::system_clock::now();

			out << m_Output << "  | TIMER STARTED";
			Logger::LogToConsole(out.str());
		}

		void End()
		{
			m_EndTime = std::chrono::system_clock::now();

			std::stringstream out;
			out << m_Output << "  | TIMER ENDED  |  ELAPSED TIME : " << std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count();
			Logger::LogToConsole(out.str());
		}
		
	private : 

		std::string m_Output;
		std::chrono::system_clock::time_point m_StartTime;
		std::chrono::system_clock::time_point m_EndTime;
	};
}