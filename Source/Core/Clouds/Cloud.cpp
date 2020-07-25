#include "Cloud.h"

namespace Minecraft
{
	CloudManager::CloudManager() : m_VBO(GL_ARRAY_BUFFER)
	{
		static bool IndexBufferInitialized = false;

		// Static index buffer
		static GLClasses::IndexBuffer StaticIBO;

		if (IndexBufferInitialized == false)
		{
			IndexBufferInitialized = true;

			GLuint* IndexBuffer = nullptr;

			int index_size = 100000; //// TODO!! THIS VALUE SHOULD BE CHANGED!!! 
			int index_offset = 0;

			IndexBuffer = new GLuint[index_size * 6];

			for (size_t i = 0; i < index_size; i += 6)
			{
				IndexBuffer[i] = 0 + index_offset;
				IndexBuffer[i + 1] = 1 + index_offset;
				IndexBuffer[i + 2] = 2 + index_offset;
				IndexBuffer[i + 3] = 2 + index_offset;
				IndexBuffer[i + 4] = 3 + index_offset;
				IndexBuffer[i + 5] = 0 + index_offset;

				index_offset = index_offset + 4;
			}

			StaticIBO.BufferData(index_size * 6 * sizeof(GLuint), IndexBuffer, GL_STATIC_DRAW);

			delete[] IndexBuffer;
		}

		/* Stride
		Position = 3 floats
		Texture Coordinate = 2 floats
		*/

		int stride = (5 * sizeof(GLfloat));

		m_VAO.Bind();
		m_VBO.Bind();
		StaticIBO.Bind();
		m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		m_VAO.Unbind();

		// Create and compile the cloud shaders
		m_CloudShader.CreateShaderProgramFromFile("Shaders/CloudShaderVert.glsl", "Shaders/CloudShaderFrag.glsl");
		m_CloudShader.CompileShaders();

		// Initialize the faces
		m_BottomFaceCoordinates[0] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		m_BottomFaceCoordinates[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_BottomFaceCoordinates[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_BottomFaceCoordinates[3] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

		m_CloudTexture.CreateTexture("Resources/Cloud.png");
		GenerateClouds();
	}

	CloudManager::~CloudManager()
	{
		m_CloudVertices.clear();
	}

	void CloudManager::Update(double time, long long frame)
	{
		static double previous_time = 0.0f;
		double delta_time = time - previous_time;
		delta_time = delta_time * 0.1f;

		// Move the clouds by the delta time or a particular velocity
		m_PositionOffset += glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);

		previous_time = time;
	}

	void CloudManager::RenderClouds(FPSCamera* camera)
	{
		m_CloudShader.Use();
		m_CloudTexture.Bind(0);
		m_CloudShader.SetInteger("u_CloudTexture", 0, 0);
		m_CloudShader.SetVector4f("u_CloudOffset", m_PositionOffset, 0);
		m_CloudShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection(), 0);

		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, floor(m_CloudVertices.size() / 4) * 6, GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();
	}

	void CloudManager::GenerateClouds()
	{
		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				AddCloudFace(glm::vec2(i, j));
			}
		}

		m_VBO.BufferData(m_CloudVertices.size() * sizeof(CloudVertex), &m_CloudVertices.front(), GL_STATIC_DRAW);
	}

	void CloudManager::AddCloudFace(const glm::vec2& position)
	{
		glm::vec3 pos = glm::vec3(position.x, 128, position.y);
		std::array<GLfloat, 8> texture_coordinates = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };;

		CloudVertex v1, v2, v3, v4;
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
		
		v1.position = translation * m_BottomFaceCoordinates[0];
		v2.position = translation * m_BottomFaceCoordinates[1];
		v3.position = translation * m_BottomFaceCoordinates[2];
		v4.position = translation * m_BottomFaceCoordinates[3];
		
		v1.texture_coords = glm::vec2(texture_coordinates[0], texture_coordinates[1]);
		v2.texture_coords = glm::vec2(texture_coordinates[2], texture_coordinates[3]);
		v3.texture_coords = glm::vec2(texture_coordinates[4], texture_coordinates[5]);
		v4.texture_coords = glm::vec2(texture_coordinates[6], texture_coordinates[7]);

		m_CloudVertices.push_back(v1);
		m_CloudVertices.push_back(v2);
		m_CloudVertices.push_back(v3);
		m_CloudVertices.push_back(v4);
	}
}