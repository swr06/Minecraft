#include <iostream>

#include "Defs.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	struct i16Vec2
	{
		uint16_t x;
		uint16_t y;
	};


	/* -- Vertex Structure --
	instead of having 3 floats for the position, texture coordinates, lighting and block face lighting, I use uint8_ts for the position and lighting. 
	I also use a uint16_t for the texture coordinates which are looked up from the fragment shader via texelFetch()
	*/

	struct Vertex
	{
		i8Vec3 position;
		uint8_t lighting_level;
		i16Vec2 texture_coords;
		uint8_t block_face_lighting;
	};

	struct ModelVertex
	{
		glm::vec3 position;
		i16Vec2 texture_coords;
		float lighting_level;
	};
}