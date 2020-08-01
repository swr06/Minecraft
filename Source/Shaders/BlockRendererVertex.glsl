#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in float a_LightingLevel;

uniform vec4 u_AmbientLight;
uniform mat4 u_ViewProjection;
uniform vec2 u_ChunkCoordinates;

out vec2 v_TexCoord;
out vec4 v_TintColor;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	v_TintColor = vec4(a_LightingLevel, a_LightingLevel, a_LightingLevel, 1.0f);
	v_TintColor = v_TintColor * u_AmbientLight;
	v_TexCoord = a_TexCoords;
}