#pragma once

// Needs glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace Minecraft
{
	// Orthographic camera class for 2D element rendering

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) 
		{ 
			m_Position = position; 
			RecalculateViewMatrix(); 
		}

		void SetRotation(float rotation) 
		{ 
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		void SetScale(const glm::vec3 &scale_factor)
		{
			m_Scale = scale_factor;
			RecalculateViewMatrix();
		}

		float GetRotation() const { return m_Rotation; }
		const glm::vec4& GetProjectionCoords()  { return m_ProjectionCoords; }
		const glm::vec3& GetPosition()  { return m_Position; }
		const glm::mat4& GetProjectionMatrix()  { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix()  { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix()  { return m_ViewProjectionMatrix; }
		const glm::vec3& GetScale() { return m_Scale; }
		const glm::mat4& GetTransformMatrix() { return m_TransformMatrix; }

	private:

		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::mat4 m_TransformMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec4 m_ProjectionCoords = { 0.0f, 0.0f, 0.0f, 0.0f }; 
		glm::vec3 m_Scale = {1.0f, 1.0f, 0.0f};

		float m_Rotation = 0.0f;
	};
}