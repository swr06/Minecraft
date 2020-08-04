#include "CloudGenerator.h"

namespace Minecraft
{
	CloudGenerator::CloudGenerator(const std::string& cloud_map_path)
	{
		m_MapData = GLClasses::ExtractTextureData(cloud_map_path);
	}

	bool CloudGenerator::ShouldCloudExistAtCoord(const glm::ivec2& coord)
	{
		int image_channels = 4;
		int element = (coord.x + (coord.y * m_MapData.width)) * image_channels;

		uint8_t r = (uint8_t)m_MapData.image_data[element];
		uint8_t g = (uint8_t)m_MapData.image_data[element + 1];
		uint8_t b = (uint8_t)m_MapData.image_data[element + 2];
		uint8_t a = (uint8_t)m_MapData.image_data[element + 3];

		if (r == 0 && g == 0 && b == 0)
		{
			return false;
		}

		else if (r == 255 && g == 255 && b == 255)
		{
			return true;
		}

		else
		{
			return false;
		}
	}
}
