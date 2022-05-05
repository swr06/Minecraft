#version 330 core
layout (location = 0) in ivec3 a_Position;
layout (location = 1) in ivec2 a_TexCoords;
layout (location = 2) in uint a_LightingLevel;
layout (location = 3) in uint a_BlockFaceLightLevel;

uniform int u_RenderDistance;
uniform vec4 u_AmbientLight;
uniform mat4 u_ViewProjection;
uniform mat4 u_ViewMatrix;
uniform float u_SunPositionY; // The normalized sun position

// To transform the positions
uniform int u_CHUNK_SIZE_X;
uniform int u_CHUNK_SIZE_Z;
uniform int u_ChunkX;
uniform int u_ChunkZ;

out float v_Visibility; // For implementing fog
out vec2 v_TexCoord;
out vec4 v_TintColor;
out float v_SunlightIntensity;
flat out int v_LightlevelRAW;
out vec3 v_WorldPosition;

const float fog_density = 0.01f;
float fog_gradient = 4.0f;

uniform float u_VertexTime;
uniform int u_VTransparent;

float WavingWater(vec3 worldPos) 
{
	float frametime = 1.0f * u_VertexTime;

	float fractY = fract(worldPos.y + 0.005);
		
	float wave = sin(6.28 * (frametime * 0.7 + worldPos.x * 0.14 + worldPos.z * 0.07)) +
				 sin(6.28 * (frametime * 0.5 + worldPos.x * 0.10 + worldPos.z * 0.20));

	if (fractY > 0.01)
	{ 
		return wave * 0.0125;
	}
	
	return 0.0;
}

void main()
{
	vec3 real_pos = vec3(a_Position.x + (u_ChunkX * u_CHUNK_SIZE_X), a_Position.y, a_Position.z + (u_ChunkZ * u_CHUNK_SIZE_Z));
	v_WorldPosition = real_pos;

	if (u_VTransparent == 1 && a_BlockFaceLightLevel > 83) {
		real_pos.y -= 0.21f;
		real_pos.y += WavingWater(real_pos * 1.5) * 10.0f;
	}

	// Calculate fog
	vec4 relative_camera_pos = u_ViewMatrix * vec4(real_pos, 1.0f);
	float fog_distance = (length(relative_camera_pos) / (u_RenderDistance)) * 5.6f;
	v_Visibility = exp(-pow((fog_distance * fog_density), fog_gradient));
	v_Visibility = clamp(v_Visibility, 0.0f, 1.0f);

	float lighting_level = float(a_LightingLevel) ;	
	lighting_level /= 24;
	lighting_level *= lighting_level;
	lighting_level *= 2.0f;

	float block_light = a_BlockFaceLightLevel < 16.0f ? float(a_BlockFaceLightLevel) : 10.0f;
	block_light /= 10.0f;

	v_SunlightIntensity = max(u_SunPositionY / 1000.0f, 0.6f);

	if (lighting_level < 0.2)
	{
		v_TintColor =  u_AmbientLight;
		v_TintColor = v_TintColor * vec4(block_light, block_light, block_light, 1.0f);
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

	v_LightlevelRAW = int(a_BlockFaceLightLevel);

	gl_Position = u_ViewProjection * vec4(real_pos, 1.0);
	v_TexCoord = vec2(a_TexCoords.xy);
}