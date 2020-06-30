#include "CubeTextureMap.h"

namespace GLClasses
{
	void CubeTextureMap::CreateCubeTextureMap(const std::vector<std::string>& cube_face_paths)
	{
		unsigned char* image_data;
		int width, height, channels;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		m_CubeFacePaths = cube_face_paths;

		for (int i = 0; i < cube_face_paths.size(); i++)
		{
			image_data = stbi_load(cube_face_paths[i].c_str(), &width, &height, &channels, 4);

			if (image_data)
			{
  				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
					height, GL_RGB, 0, GL_UNSIGNED_BYTE, &image_data);
				stbi_image_free(image_data);
			}

			else
			{
				std::cout <<"\nFailed to load image : " << cube_face_paths[i] << "\n\n\n" ;
				stbi_image_free(image_data);
			}
		}

		// Setting some image parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
}