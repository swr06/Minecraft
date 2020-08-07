#include "Renderer.h"

namespace Minecraft
{
	Renderer::Renderer() : m_VBO(GL_ARRAY_BUFFER)
	{
		// Manage the vertex arrays

		m_DefaultShader.CreateShaderProgramFromFile("Shaders/BlockRendererVertex.glsl", "Shaders/BlockRendererFrag.glsl");
		m_DefaultShader.CompileShaders();
		m_BlockAtlas.CreateTexture("Resources/BlockAtlasHighDef.png");
	}

	void Renderer::RenderChunk(Chunk* chunk)
	{
		if (chunk->GetChunkMesh()->p_VerticesCount > 0)
		{
			chunk->GetChunkMesh()->p_VAO.Bind();
			DebugGLFunction(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_VerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
			chunk->GetChunkMesh()->p_VAO.Unbind();
		}
	}

	void Renderer::RenderTransparentChunk(Chunk* chunk)
	{
		if (chunk->GetChunkMesh()->p_TransparentVerticesCount > 0)
		{
			chunk->GetChunkMesh()->p_TransparentVAO.Bind();
			DebugGLFunction(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_TransparentVerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
			chunk->GetChunkMesh()->p_TransparentVAO.Unbind();
		}
	}

	void Renderer::StartChunkRendering(FPSCamera* camera, const glm::vec4& ambient_light, int render_distance)
	{
		m_DefaultShader.Use();
		m_BlockAtlas.Bind(0);
		m_DefaultShader.SetInteger("u_Texture", 0, 0);
		m_DefaultShader.SetVector4f("u_AmbientLight", ambient_light, 0);
		m_DefaultShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
		m_DefaultShader.SetMatrix4("u_ViewMatrix", camera->GetViewMatrix());
		m_DefaultShader.SetInteger("u_RenderDistance", render_distance);
		//m_DefaultShader.SetVector4f("u_FogColor", glm::vec4(0.60f, 0.79f, 0.89f, 1.0f)); // BLUE FOG
		m_DefaultShader.SetVector4f("u_FogColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // WHITE FOG
	}

	void Renderer::EndChunkRendering()
	{
		// Rendering ended
	}
}