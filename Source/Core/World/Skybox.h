#pragma once

#include <iostream>

#include "../OpenGL Classes/CubeTextureMap.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../FpsCamera.h"

namespace Minecraft
{
	class Skybox
	{
	public:
		Skybox();
		void RenderSkybox(FPSCamera* camera, const glm::vec4& sun_pos);

	private :
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::Shader m_SkyboxShader;
		GLClasses::CubeTextureMap m_CubeMap;
	};
}