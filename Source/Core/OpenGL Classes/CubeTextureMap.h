#pragma once

#include "stb_image.h"

#include "GL/glew.h"

#include <string>
#include <vector>
#include <iostream>

namespace GLClasses
{
	using namespace std;

	class CubeTextureMap
	{
	public : 

		void CreateCubeTextureMap(const std::vector<std::string>& cube_face_paths);
		inline GLuint GetID() const { return m_TextureID; }

	private :

		GLuint m_TextureID;
		int m_Width;
		int m_Height;
		int m_Channels;
		
		std::vector<std::string> m_CubeFacePaths;
	};
}