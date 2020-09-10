#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;
out float v_SunlightIntensity;

uniform float u_SunPositionY;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_TexCoords = a_Position;
	v_SunlightIntensity = max(u_SunPositionY / 1500.0f, 0.30);
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}  