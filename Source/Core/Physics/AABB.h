#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft
{
	class AABB
	{
	private:
		glm::vec3 m_Min, m_Max;

	public:
		AABB(const glm::vec3& min, const glm::vec3& max);

		bool IsPointInsideAABB(const glm::vec3& position, const glm::vec3& point) const;
		bool Intersects(const AABB& aabb, const glm::vec3& positionOne, const glm::vec3& positionTwo) const;

		inline glm::vec3 GetMinimum() const { return m_Min; }
		inline glm::vec3 GetMaximum() const { return m_Max; }
		inline glm::vec3 GetRelativeMinimum(const glm::vec3& position) const { return m_Min + position; }
		inline glm::vec3 GetRelativeMaximum(const glm::vec3& position) const { return m_Max + position; }
	};
}