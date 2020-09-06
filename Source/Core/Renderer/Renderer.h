#pragma once
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h>

#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/Texture.h"
#include "../OpenGL Classes/GLDebug.h"

#include "../Chunk.h"
#include "../FpsCamera.h"

namespace Minecraft
{
	class Renderer
	{
	public : 

		Renderer();

		void StartChunkRendering(FPSCamera* camera, const glm::vec4& ambient_light, int render_distance, const glm::vec4& sun_position);
		void RenderTransparentChunk(Chunk* chunk);
		void RenderChunk(Chunk* chunk);
		void EndChunkRendering();

		void StartChunkModelRendering(FPSCamera* camera, const glm::vec4& ambient_light, int render_distance, const glm::vec4& sun_position);
		void RenderChunkModels(Chunk* chunk);
		void EndChunkModelRendering();

		GLClasses::Texture* GetAtlasTexture() { return &m_BlockAtlas; }

	private: 
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::Shader m_DefaultChunkShader;
		GLClasses::Shader m_DefaultChunkModelShader;
		GLClasses::Texture m_BlockAtlas;
	};
}