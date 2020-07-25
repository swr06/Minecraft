#include "Cloud.h"

namespace Minecraft
{
	CloudManager::CloudManager(FPSCamera* camera) : m_VBO(GL_ARRAY_BUFFER)
	{
		static bool IndexBufferInitialized = false;

		// Static index buffer
		static GLClasses::IndexBuffer StaticIBO;

		if (IndexBufferInitialized == false)
		{
			IndexBufferInitialized = true;

			GLuint* IndexBuffer = nullptr;

			int index_size = 2000;
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

		// Initialize the faces
		m_Faces[0] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		m_Faces[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_Faces[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Faces[3] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	}

	CloudManager::~CloudManager()
	{
		for (int i = 0; i < m_CloudVertices.size(); i++)
		{
			m_CloudVertices.at(i).clear();
		}
	}

	void CloudManager::Update(double time, long long frame)
	{
		static double previous_time = 0.0f;
		double delta_time = time - previous_time;

		// Move the clouds by the delta time
		
		previous_time = time;
	}

	void CloudManager::RenderClouds()
	{
		
	}

	void CloudManager::AddCloudFace(const glm::vec3& position)
	{

	}
}