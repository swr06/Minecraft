#include "AABB.h"

namespace Minecraft
{
	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
		: m_Min(min), m_Max(max)
	{
	}

	bool AABB::IsPointInsideAABB(const glm::vec3& position, const glm::vec3& point) const
	{
		glm::vec3 oneMin = GetRelativeMinimum(position),
			oneMax = GetRelativeMaximum(position);

		return (point.x >= oneMin.x && point.x <= oneMax.x)
			&& (point.y >= oneMin.y && point.y <= oneMax.y)
			&& (point.z >= oneMin.z && point.z <= oneMax.z);
	}

	bool AABB::Intersects(const AABB& aabb, const glm::vec3& positionOne, const glm::vec3& positionTwo) const
	{
		glm::vec3 oneMin = GetRelativeMinimum(positionOne),
			oneMax = GetRelativeMaximum(positionOne),
			twoMin = aabb.GetRelativeMinimum(positionTwo),
			twoMax = aabb.GetRelativeMaximum(positionTwo);

		return (oneMin.x <= twoMax.x && oneMax.x >= twoMin.x)
			&& (oneMin.y <= twoMax.y && oneMax.y >= twoMin.y)
			&& (oneMin.z <= twoMax.z && oneMax.z >= twoMin.z);
	}
}