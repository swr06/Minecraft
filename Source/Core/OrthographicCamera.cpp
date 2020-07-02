#include "OrthographicCamera.h"

namespace Minecraft
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_ViewMatrix = glm::mat4(1.0f); 
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
		m_ViewProjectionMatrix = m_ProjectionMatrix ;
		m_ProjectionCoords = { left, right, bottom, top };
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionCoords = { left, right, bottom, top }; 
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		transform = glm::inverse(transform);
		transform = glm::scale(glm::mat4(transform), glm::vec3(m_Scale));

		glm::mat4 transform_ = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		transform_ = glm::inverse(transform_);
		transform_ = glm::scale(glm::mat4(transform_), glm::vec3(m_Scale));
		
		 m_TransformMatrix = transform_;

		m_ViewMatrix = transform;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}