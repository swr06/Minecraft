#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// A Simple 3D camera

namespace Minecraft
{
	class Camera
	{
	public : 
		Camera(float fov, float aspect, float zNear, float zFar); 
		~Camera();

		void SetPosition(const glm::vec3& position);
		void ChangePosition(const glm::vec3& position_increment);
		void SetFront(const glm::vec3& front);
		void SetRotation(float angle);
		void SetFov(float fov);
		void SetAspect(float aspect);
		void SetNearAndFarPlane(float zNear, float zFar);
		void SetPerspectiveMatrix(float fov, float aspect_ratio, float zNear, float zFar);

		inline const glm::vec3& GetPosition()
		{
			return m_Position;
		}

		inline float GetFov()
		{
			return m_Fov;
		}

		inline float GetRotation()
		{
			return m_Rotation;
		}

		inline const glm::mat4& GetViewProjection()
		{
			return m_ViewProjectionMatrix;
		}

		inline const glm::mat4& GetViewMatrix()
		{
			return m_ViewMatrix;
		}

		inline const glm::vec3& GetFront()
		{
			return m_Front;
		}

		inline const glm::vec3& GetUp()
		{
			return m_Up;
		}

		inline const glm::vec3 GetRight()
		{
			return glm::normalize(glm::cross(m_Front, m_Up));
		}

		void RefreshCamera()
		{
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}

	private : 

		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

		float m_Rotation;
		float m_Fov;
		float m_Aspect;
		float m_zNear;
		float m_zFar;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
}