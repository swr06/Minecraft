#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../FpsCamera.h"

namespace Minecraft
{
	class RayCaster
	{
	public :
		void CastRay(float mx, float my, float window_width, float window_height)
		{
			glm::vec2 mouse_coords = ConvertToNDC(mx, my, window_width, window_height);
			glm::vec4 clip_coords = glm::vec4(mouse_coords.x, mouse_coords.y, -1.0f, 1.0f);
		}

	private:

		glm::vec2 ConvertToNDC(float mx, float my, float ww, float wh)
		{
			glm::vec2 ret_val;

			ret_val.x = (2 * mx) / ww - 1.0f;
			ret_val.y = (2 * my) / wh - 1.0f;
			ret_val.y = -ret_val.y;
		
			return ret_val;
		}

		glm::vec3 m_Ray;
		FPSCamera* m_Camera;
	};
}