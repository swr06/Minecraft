#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in float a_LightingLevel;

uniform int u_RenderDistance;
uniform vec4 u_AmbientLight;
uniform mat4 u_ViewProjection;
uniform mat4 u_ViewMatrix;
uniform vec2 u_ChunkCoordinates;
uniform float u_SunPositionY;
uniform float u_Time;

out float v_Visibility;
out vec2 v_TexCoord;
out vec4 v_TintColor;
out float v_SunlightIntensity;

const float fog_density = 0.01f;
float fog_gradient = float(u_RenderDistance + 2.50f);

void main()
{
	vec4 pos = vec4(a_Position, 1.0f);

    pos.x += sin((u_Time + pos.z + pos.y) * 1.8f) / 15.0f;
    pos.z -= cos((u_Time + pos.x + pos.y) * 1.8f) / 15.0f;

	// Calculate fog
	vec4 relative_camera_pos = u_ViewMatrix * vec4(a_Position, 1.0f);
	float fog_distance = length(relative_camera_pos);
	v_Visibility = exp(-pow((fog_distance * fog_density), fog_gradient));
	v_Visibility = clamp(v_Visibility, 0.0f, 1.0f);

	float lighting_level = a_LightingLevel ;	
	lighting_level /= 10;

	v_SunlightIntensity = max(u_SunPositionY / 1000.0f, 0.6f);

	if (lighting_level < 0.2)
	{
		v_TintColor =  u_AmbientLight;
	}

	else 
	{
		if (lighting_level < u_AmbientLight.x)
		{
			lighting_level = u_AmbientLight.x;
		}

		v_TintColor = vec4(lighting_level, lighting_level, lighting_level, 1.0f);

		if (v_SunlightIntensity < 1.0f)
		{
			v_SunlightIntensity = 1.0f;
		}
	}

	gl_Position = u_ViewProjection * pos;
	v_TexCoord = a_TexCoords;
}