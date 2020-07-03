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
			m_Yaw = camera->GetYaw();
			m_Pitch = camera->GetPitch();

			m_Direction = camera->GetFront();
		}

		void StepRay(float scale)
		{
			m_RayEnd.x -= glm::cos(m_Yaw) * scale;
			m_RayEnd.z -= glm::sin(m_Yaw) * scale;
			m_RayEnd.y -= glm::tan(m_Pitch) * scale;
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

		glm::vec3 m_RayStart;
		glm::vec3 m_RayEnd;
		glm::vec3 m_Direction;
		float m_Yaw;
		float m_Pitch;
	};
}