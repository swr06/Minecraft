#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in float a_LightingLevel;
layout (location = 3) in float a_BlockFaceLightLevel;

uniform vec4 u_AmbientLight;
uniform mat4 u_ViewProjection;
uniform vec2 u_ChunkCoordinates;

out vec2 v_TexCoord;
out vec4 v_TintColor;

void main()
{
	float lighting_level = a_LightingLevel / 10;

	if (lighting_level < 0.2)
	{
		v_TintColor =  u_AmbientLight;
		v_TintColor = v_TintColor * vec4(a_BlockFaceLightLevel, a_BlockFaceLightLevel, a_BlockFaceLightLevel, 1.0f);
	}

	else 
	{
		if (lighting_level < u_AmbientLight.x)
		{
			lighting_level = u_AmbientLight.x;
		}

		v_TintColor = vec4(lighting_level, lighting_level, lighting_level, 1.0f);
	}


	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoords;
}