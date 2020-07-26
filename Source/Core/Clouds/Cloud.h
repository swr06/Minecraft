#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <array>

#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/IndexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/Texture.h"

#include "CloudGenerator.h"
#include "../Utils/Defs.h"

#include "../FpsCamera.h"

namespace Minecraft
{
	enum CloudDriftDirections
	{
		North,
		South,
		East,
		West
	};

	class CloudManager
	{
	public :
		CloudManager();
		~CloudManager();

		void Update(double time, long long frame);
		void RenderClouds(FPSCamera* camera, const glm::vec3& player_position);

	private:

		void GenerateClouds();

		struct CloudVertex
		{
			glm::vec3 position;
			glm::vec2 texture_coords;
		};

		void AddCloudFace(const glm::vec2& position);

		// The maximum amount of cloud chunks that can be there on the screen
		std::vector<CloudVertex> m_CloudVertices;

		// OpenGL specific objects
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		glm::vec4 m_BottomFaceCoordinates[4];

		glm::vec4 m_PositionOffset;

		GLClasses::Shader m_CloudShader;
		GLClasses::Texture m_CloudTexture;
		CloudGenerator m_CloudGenerator;
	};

}