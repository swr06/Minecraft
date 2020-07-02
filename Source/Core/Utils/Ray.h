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
			m_RayStart = camera->GetPosition();
			m_RayEnd = camera->GetPosition();
			m_Direction = camera->GetFront();
		}

		void StepRay(float scale)
		{
			float yaw = glm::radians(m_Direction.y + 90);
			float pitch = glm::radians(m_Direction.x);

			m_RayEnd.x -= glm::cos(yaw) * scale;
			m_RayEnd.z -= glm::sin(yaw) * scale;
			m_RayEnd.y -= glm::tan(pitch) * scale;
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
	};
}