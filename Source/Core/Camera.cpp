#include "Camera.h"

namespace Minecraft
{
	Camera::Camera(float fov, float aspect, float zNear, float zFar)
	{
		m_Fov = fov;
		m_Aspect = aspect;
		m_zNear = zNear;
		m_zFar = zFar;

		m_Rotation = 0.0f;
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

		m_ViewMatrix = glm::lookAt(m_Position, m_Front + m_Position, m_Up);
		m_ProjectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	Camera::~Camera()
	{
		// Nothing to do here
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;

		RecalculateViewMatrix();
	}

	void Camera::ChangePosition(const glm::vec3& position_increment)
	{
		m_Position.x += position_increment.x;
		m_Position.y += position_increment.y;
		m_Position.z += position_increment.z;
		//m_Position = m_Position + position_increment;

		RecalculateViewMatrix();
	}

	void Camera::SetRotation(float angle)
	{
		m_Rotation = angle;

		RecalculateViewMatrix();
	}

	void Camera::SetFov(float fov)
	{
		m_Fov = fov;

		RecalculateProjectionMatrix();
	}

	void Camera::SetAspect(float aspect)
	{
		m_Aspect = aspect;

		RecalculateProjectionMatrix();
	}

	void Camera::SetNearAndFarPlane(float zNear, float zFar)
	{
		m_zNear = zNear;
		m_zFar = zFar;

		RecalculateProjectionMatrix();
	}

	void Camera::SetPerspectiveMatrix(float fov, float aspect_ratio, float zNear, float zFar)
	{
		m_Fov = fov;
		m_Aspect = aspect_ratio;
		m_zNear = zNear;
		m_zFar = zFar;

		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Front + m_Position, m_Up);
		m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation), glm::vec3(1.0f, 0.5f, 0.5f));

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_zNear, m_zFar);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
