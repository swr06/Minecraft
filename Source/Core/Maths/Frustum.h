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
		void UpdateCameraFrustum();

	private : 

		float m_FOV;
		float m_NearPlane;
		float m_FarPlane;
		float m_Aspect;

		float m_NearWidth;
		float m_NearHeight;
		float m_FarWidth;
		float m_FarHeight;

		float m_Tangent;

		glm::vec3 m_Position;
		glm::vec3 m_Direction;
	};
}