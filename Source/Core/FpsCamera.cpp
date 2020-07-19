#include "FpsCamera.h"

namespace Minecraft
{
	FPSCamera::FPSCamera(float fov, float aspect, float zNear, float zFar, float sensitivity)
	{
		m_Fov = fov;
		m_Aspect = aspect;
		m_zNear = zNear;
		m_zFar = zFar;

		m_Rotation = 0.0f;
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

		_Pitch = 0.0f;
		_Yaw = 0.0f;

		m_ViewMatrix = glm::lookAt(m_Position, m_Front + m_Position, m_Up);
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	FPSCamera::~FPSCamera()
	{
		// Nothing to do here
	}

	void FPSCamera::UpdateOnMouseMovement(double xpos, double ypos)
	{
		ypos = -ypos;

		float x_diff = xpos - _PrevMx;
		float y_diff = ypos - _PrevMy;

		if (_FirstMove == false)
		{
			_FirstMove = true;
			_PrevMx = xpos;
			_PrevMy = ypos;
		}

		x_diff = x_diff * _Sensitivity;
		y_diff = y_diff * _Sensitivity;

		_PrevMx = xpos;
		_PrevMy = ypos;

		_Yaw = _Yaw + x_diff;
		_Pitch = _Pitch + y_diff;

		if (_Pitch > 89.0f)
		{
			_Pitch = 89.0f;
		}

		if (_Pitch < -89.0f)
		{
			_Pitch = -89.0f;
		}

		glm::vec3 front;

		front.x = cos(glm::radians(_Pitch)) * cos(glm::radians(_Yaw));
		front.y = sin(glm::radians(_Pitch));
		front.z = cos(glm::radians(_Pitch)) * sin(glm::radians(_Yaw));

		this->SetFront(front);
	}

	void FPSCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;

		RecalculateViewMatrix();
	}

	void FPSCamera::ChangePosition(const glm::vec3& position_increment)
	{
		m_Position = m_Position + position_increment;

		RecalculateViewMatrix();
	}

	void FPSCamera::SetFront(const glm::vec3& front)
	{
		m_Front = front;
		RecalculateViewMatrix();
	}

	void FPSCamera::SetRotation(float angle)
	{
		m_Rotation = angle;

		RecalculateViewMatrix();
	}

	void FPSCamera::SetFov(float fov)
	{
		m_Fov = fov;

		RecalculateProjectionMatrix();
	}

	void FPSCamera::SetAspect(float aspect)
	{
		m_Aspect = aspect;

		RecalculateProjectionMatrix();
	}

	void FPSCamera::SetNearAndFarPlane(float zNear, float zFar)
	{
		m_zNear = zNear;
		m_zFar = zFar;

		RecalculateProjectionMatrix();
	}

	void FPSCamera::SetPerspectiveMatrix(float fov, float aspect_ratio, float zNear, float zFar)
	{
		m_Fov = fov;
		m_Aspect = aspect_ratio;
		m_zNear = zNear;
		m_zFar = zFar;

		RecalculateProjectionMatrix();
	}

	void FPSCamera::MoveCamera(MoveDirection dir, float camera_speed)
	{
		switch (dir)
		{
			case MoveDirection::Front:
				ChangePosition(glm::vec3(GetFront().x * camera_speed, 0.0f, GetFront().z * camera_speed));
				break;

			case MoveDirection::Back:	
				ChangePosition(-(GetFront() * camera_speed));
				break;

			case MoveDirection::Left : 
				ChangePosition(-(GetRight() * camera_speed));
				break;

			case MoveDirection::Right :
				ChangePosition(GetRight() * camera_speed);
				break;

			case MoveDirection::Up :
				ChangePosition(GetUp() * camera_speed);
				break;

			case MoveDirection::Down :
				ChangePosition(-(GetUp() * camera_speed));
				break;
		}
	}

	void FPSCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Front + m_Position, m_Up);
		m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation), glm::vec3(1.0f, 0.5f, 0.5f));

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void FPSCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_zNear, m_zFar);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
