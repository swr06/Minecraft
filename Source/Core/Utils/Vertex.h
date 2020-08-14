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

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		GLfloat lighting_level; 
		GLfloat block_face_lighting;
	};

	struct FloraVertex
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		GLfloat lighting_level;
	};
}