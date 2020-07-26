#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../OpenGL Classes/Texture.h"

namespace Minecraft
{
	class CloudGenerator
	{
	public :

		CloudGenerator(const std::string& cloud_map_path);

		// Checks a particular pixel in the cloud map and determines if there has to be a cloud there or not
		bool ShouldCloudExistAtCoord(const glm::ivec2& coord);

	private:

		std::string m_MapPath;
		GLClasses::ExtractedImageData m_MapData;
	};
}