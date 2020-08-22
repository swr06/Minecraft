#include <iostream>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Defs.h"

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

	class i8Vec3
	{
	public :
		uint8_t x;
		uint8_t y;
		uint8_t z;

		i8Vec3 operator=(const glm::vec3& vec)
		{
			i8Vec3 ret_val;
			assert(floor(vec.x) <= CHUNK_SIZE_X);
			assert(floor(vec.y) < 255);
			assert(floor(vec.z) <= CHUNK_SIZE_Z);

			ret_val.x = floor(vec.x);
			ret_val.y = floor(vec.y);
			ret_val.z = floor(vec.z);

			return ret_val;
		}
	};

	struct i16Vec2
	{
		uint16_t x;
		uint16_t y;
	};

	struct Vertex
	{
		i8Vec3 position;
		glm::vec2 texture_coords;
		uint8_t lighting_level; 
		float block_face_lighting;
	};

	struct ModelVertex
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		float lighting_level;
	};
}