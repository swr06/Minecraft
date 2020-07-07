#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../FpsCamera.h"

namespace Minecraft
{
	class Frustum
	{
		Frustum(FPSCamera* camera);

	private : 

		float m_FOV;
		float m_NearPlane;
		float m_FarPlane;
		float m_Aspect;
		glm::vec3 m_Position;
		glm::vec3 m_Direction;
	};
}