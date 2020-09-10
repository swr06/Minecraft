#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ProjectionMatrix;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ProjectionMatrix * vec4(a_Position.x, a_Position.y, 1.0f, 1.0f);
}