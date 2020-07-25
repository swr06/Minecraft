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

#include "../Utils/Vertex.h"
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
		CloudManager(FPSCamera* camera);
		~CloudManager();

		void Update(double time, long long frame);
		void RenderClouds();

	private:

		struct CloudVertex
		{
			glm::vec3 position;
			glm::vec2 texture_coords;
		};


		void AddCloudFace(const glm::vec3& position);

		// The maximum amount of cloud chunks that can be there on the screen
		std::array<std::vector<Vertex>, 16> m_CloudVertices;

		// OpenGL specific objects
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		glm::vec4 m_Faces[4];

		glm::mat4 m_ModelMatrix;
	};

}