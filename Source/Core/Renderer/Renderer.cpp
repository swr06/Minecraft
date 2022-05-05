#include "Renderer.h"

namespace Minecraft
{
	Renderer::Renderer() : m_VBO(GL_ARRAY_BUFFER)
	{
		// Create and compile the shaders

		m_DefaultChunkShader.CreateShaderProgramFromFile("Shaders/BlockRendererVertex.glsl", "Shaders/BlockRendererFrag.glsl");
		m_DefaultChunkModelShader.CreateShaderProgramFromFile("Shaders/ModelRendererVertex.glsl", "Shaders/ModelRendererFrag.glsl");
		m_DefaultChunkShader.CompileShaders();
		m_DefaultChunkModelShader.CompileShaders();

		m_BlockAtlas.CreateTexture("Resources/64x64_sheet.png");
	}

	void Renderer::RenderChunk(Chunk* chunk)
	{
		if (chunk->GetChunkMesh()->p_VerticesCount > 0)
		{
			m_DefaultChunkShader.SetInteger("u_ChunkX", chunk->p_Position.x);
			m_DefaultChunkShader.SetInteger("u_ChunkZ", chunk->p_Position.z);

			chunk->GetChunkMesh()->p_VAO.Bind();
			(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_VerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
			chunk->GetChunkMesh()->p_VAO.Unbind();
		}
	}

	void Renderer::RenderTransparentChunk(Chunk* chunk)
	{
		if (chunk->GetChunkMesh()->p_TransparentVerticesCount > 0)
		{
			m_DefaultChunkShader.SetInteger("u_ChunkX", chunk->p_Position.x);
			m_DefaultChunkShader.SetInteger("u_ChunkZ", chunk->p_Position.z);

			chunk->GetChunkMesh()->p_TransparentVAO.Bind();
			(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_TransparentVerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
			chunk->GetChunkMesh()->p_TransparentVAO.Unbind();
		}
	}

	const glm::vec4 FogColor = glm::vec4((glm::vec3(151.0f, 183.0f, 245.0f) / 255.0f)*1.1f, 1.0f);


	void Renderer::StartChunkRendering(FPSCamera* camera, const glm::vec4& ambient_light, int render_distance, const glm::vec4& sun_position)
	{
		m_DefaultChunkShader.Use();
		m_BlockAtlas.Bind(0);
		m_DefaultChunkShader.SetInteger("u_Texture", 0, 0);
		m_DefaultChunkShader.SetVector4f("u_AmbientLight", ambient_light, 0);
		m_DefaultChunkShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
		m_DefaultChunkShader.SetMatrix4("u_ViewMatrix", camera->GetViewMatrix());
		m_DefaultChunkShader.SetInteger("u_RenderDistance", render_distance);
		m_DefaultChunkShader.SetInteger("u_CHUNK_SIZE_X", CHUNK_SIZE_X);
		m_DefaultChunkShader.SetInteger("u_CHUNK_SIZE_Z", CHUNK_SIZE_Z);
		m_DefaultChunkShader.SetFloat("u_SunPositionY", sun_position.y);
		m_DefaultChunkShader.SetVector4f("u_FogColor", FogColor); // WHITE FOG
	}

	void Renderer::EndChunkRendering()
	{
		glUseProgram(0);
	}

	void Renderer::StartChunkModelRendering(FPSCamera* camera, const glm::vec4& ambient_light, int render_distance, const glm::vec4& sun_position)
	{
		m_DefaultChunkModelShader.Use();
		m_BlockAtlas.Bind(0);
		m_DefaultChunkModelShader.SetInteger("u_Texture", 0, 0);
		m_DefaultChunkModelShader.SetVector4f("u_AmbientLight", ambient_light, 0);
		m_DefaultChunkModelShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
		m_DefaultChunkModelShader.SetMatrix4("u_ViewMatrix", camera->GetViewMatrix());
		m_DefaultChunkModelShader.SetInteger("u_RenderDistance", render_distance);
		m_DefaultChunkModelShader.SetFloat("u_SunPositionY", sun_position.y);
		m_DefaultChunkShader.SetInteger("u_CHUNK_SIZE_X", CHUNK_SIZE_X);
		m_DefaultChunkShader.SetInteger("u_CHUNK_SIZE_Z", CHUNK_SIZE_Z);
		m_DefaultChunkModelShader.SetVector4f("u_FogColor", FogColor); // WHITE FOG
		m_DefaultChunkModelShader.SetFloat("u_Time", glfwGetTime(), 0);
	}

	void Renderer::RenderChunkModels(Chunk* chunk)
	{
		if (chunk->GetChunkMesh()->p_ModelVerticesCount > 0)
		{
			m_DefaultChunkShader.SetInteger("u_ChunkX", chunk->p_Position.x);
			m_DefaultChunkShader.SetInteger("u_ChunkZ", chunk->p_Position.z);
			chunk->GetChunkMesh()->p_ModelVAO.Bind();
			(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_ModelVerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
			chunk->GetChunkMesh()->p_ModelVAO.Unbind();
		}
	}

	void Renderer::EndChunkModelRendering()
	{
		glUseProgram(0);
	}
}