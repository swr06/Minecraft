#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform vec4 u_CloudOffset;

out vec2 v_TexCoord;

void main()
{
	vec4 position = vec4(a_Position, 1.0f);
	position = position + u_CloudOffset;

	gl_Position = u_ViewProjection * position;
	v_TexCoord = a_TexCoords;
}