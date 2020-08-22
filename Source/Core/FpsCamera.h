#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// The FPS Camera that is used for the player's movement, frustum culling

namespace Minecraft
{
	class FPSCamera
	{
	public : 
		FPSCamera(float fov, float aspect, float zNear, float zFar, float sensitivity = 0.25f);
		~FPSCamera();

		void UpdateOnMouseMovement(double xpos, double ypos);
		void SetPosition(const glm::vec3& position);
		void ChangePosition(const glm::vec3& position_increment);
		void SetFront(const glm::vec3& front);
		void SetRotation(float angle);
		void SetFov(float fov);
		void SetAspect(float aspect);
		void SetNearAndFarPlane(float zNear, float zFar);
		void SetPerspectiveMatrix(float fov, float aspect_ratio, float zNear, float zFar);

		inline float GetYaw()
		{
			return _Yaw;
		}

		inline float GetPitch()
		{
			return _Pitch;
		}

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

		inline const glm::mat4& GetProjectionMatrix()
		{
			return m_ProjectionMatrix;
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

		inline float GetAspect()
		{
			return m_Aspect;
		}

		inline float GetNearPlane()
		{
			return m_zNear;
		}

		inline float GetFarPlane()
		{
			return m_zFar;
		}

		inline void ResetAcceleration()
		{
			m_Acceleration = glm::vec3(0.0f);
		}

		inline void ResetVelocity()
		{
			m_Velocity = glm::vec3(0.0f);
		}

		inline void ApplyAcceleration(const glm::vec3& acceleration)
		{
			m_Acceleration = m_Acceleration + acceleration;
		}

		void Refresh()
		{
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}
		void OnUpdate();

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

		glm::vec3 m_Acceleration;
		glm::vec3 m_Velocity;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		// The yaw and pitch of the camera : 
		bool _FirstMove = false;
		float _Sensitivity = 0.2;
		float _PrevMx = 0.0f;
		float _PrevMy = 0.0f;
		float _Yaw = 0.0f;
		float _Pitch = 0.0f;
	};
}