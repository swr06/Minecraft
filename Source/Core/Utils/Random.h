#pragma once

#include <random>

// Fixes error when windows.h is included
#undef max

class Random
{
public:
	Random(unsigned int seed = std::random_device()())
	{
		m_Engine.seed(seed);
	}

	float Float()
	{
		return (float)m_Distribution(m_Engine) / (float)std::numeric_limits<uint32_t>::max();
	}

	int Int(int limit)
	{
		return (int)m_Distribution(m_Engine) % limit;
	}

	unsigned int UnsignedInt(unsigned int limit)
	{
		return (unsigned int)m_Distribution(m_Engine) % limit;
	}

private:

	// Uses the std::mt19937 random engine
	std::mt19937 m_Engine;
	std::uniform_int_distribution<std::mt19937::result_type> m_Distribution;
};