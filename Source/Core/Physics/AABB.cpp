#include "AABB.h"

namespace Minecraft
{
	bool TestAABBCollision(const AABB& box1, const AABB& box2)
	{
		if (box1.x < box2.x + box2.width &&
			box1.x + box1.width > box2.x &&
			box1.y < box2.y + box2.height &&
			box1.y + box1.height > box2.y &&
			box1.z < box2.z + box2.depth &&
			box1.z + box1.depth > box2.z)
		{
			return true;
		}

		else
		{
			return false;
		}
	}
}