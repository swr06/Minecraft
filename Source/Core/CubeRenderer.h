#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL Classes\VertexBuffer.h"
#include "OpenGL Classes\VertexArray.h"
#include "OpenGL Classes\IndexBuffer.h"
#include "OpenGL Classes\Shader.h"
#include "OpenGL Classes\Texture.h"

using namespace GLClasses;

namespace Minecraft
{
	struct Cube
	{
		float length;
		float height;
		float breadth;
	};

	class CubeRenderer
	{
	public :
		CubeRenderer();
		~CubeRenderer();
		void RenderCube(const glm::vec3& position, Texture* texture, float rotation, const glm::mat4& projection, const glm::mat4& view = glm::mat4(1.0f), Shader* shader = nullptr);

	private :

		VertexBuffer m_VBO;
		IndexBuffer m_IBO;
		VertexArray m_VAO;
		Shader m_DefaultShader;
	};
}