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
		chunk->GetChunkMesh()->p_VAO.Bind();
		DebugGLFunction(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_VerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
		chunk->GetChunkMesh()->p_VAO.Unbind();
	}

	void Renderer::StartChunkRendering(FPSCamera* camera, const glm::vec4& ambient_light)
	{
		m_DefaultShader.Use();
		m_BlockAtlas.Bind(0);
		m_DefaultShader.SetInteger("u_Texture", 0, 0);
		m_DefaultShader.SetVector4f("u_AmbientLight", ambient_light, 0);
		m_DefaultShader.SetMatrix4("u_Model", glm::mat4(1.0f));
		m_DefaultShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
	}

	void Renderer::EndChunkRendering()
	{
		// Rendering ended
	}
}