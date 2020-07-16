#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../FpsCamera.h"

namespace Minecraft
{
	class RayCast
	{
	public:

		RayCast(FPSCamera* camera)
		{
			glm::vec3 origin = camera->GetPosition();
			m_RayStart = origin;
			m_RayEnd = origin;
			m_Yaw = glm::radians(camera->GetYaw());
			m_Pitch = glm::radians(camera->GetPitch() + 90.0f);
			m_Direction = camera->GetFront();
		}

		void StepRay(float scale)
		{
			float yaw = glm::radians(m_Direction.y + 90);
			float pitch = glm::radians(m_Direction.x);

			auto& p = m_RayEnd;

			p.x -= glm::cos(yaw) * scale;
			p.z -= glm::sin(yaw) * scale;
			p.y -= glm::tan(pitch) * scale;
		}

		inline const glm::vec3& GetEnd() const
		{
			return m_RayEnd;
		}

		float GetLength()
		{
			return glm::distance(m_RayStart, m_RayEnd);
		}

	private:

		glm::vec3 m_RayStart; // Origin of the ray
		glm::vec3 m_RayEnd;
		glm::vec3 m_Direction;
		float m_Yaw;
		float m_Pitch;
	};
}