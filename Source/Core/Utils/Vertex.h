#include <iostream>

namespace Minecraft
{
	/* Block faces
	0 = Top
	1 = Bottom
	2 = Front
	3 = Back
	4 = Right
	5 = Left
	< 6 | > 6 = Invalid
	*/

	struct i8Vec3
	{
		uint8_t x;
		uint8_t y;
		uint8_t z;

		i8Vec3 operator=(const glm::vec3& vec)
		{
			assert(floor(vec.x) <= CHUNK_SIZE_X);
			assert(floor(vec.y) < 255);
			assert(floor(vec.z) <= CHUNK_SIZE_Z);

			x = floor(vec.x);
			y = floor(vec.y);
			z = floor(vec.z);

			return *this;
		}
	};

	struct Vertex
	{
		i8Vec3 position;
		glm::vec2 texture_coords;
		uint8_t lighting_level; 
		uint8_t block_face_lighting;
	};

	struct ModelVertex
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		float lighting_level;
	};
}