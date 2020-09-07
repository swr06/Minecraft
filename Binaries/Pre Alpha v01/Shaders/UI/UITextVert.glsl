#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ProjectionMatrix;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ProjectionMatrix * vec4(a_Position.x, a_Position.y, 1.0f, 1.0f);
}