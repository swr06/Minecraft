#version 330 core
in float v_Visibility;
in vec2 v_TexCoord;
in vec4 v_TintColor;
in float v_SunlightIntensity;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec4 u_FogColor;

vec3 SkyTint = vec3(151.0f, 183.0f, 245.0f) / 255.0f;


vec3 RomBinDaHouseToneMapping(vec3 color)
{
    color = exp( -1.0 / ( 2.72*color + 0.15 ) );
	color = pow(color, vec3(1. / 2.5));
	return color;
}

void main()
{
	const float threshhold = 0.1;
	o_Color =  texelFetch(u_Texture, ivec2(v_TexCoord.xy), 0) ;
	
	o_Color.xyz = pow(o_Color.xyz, vec3(2.2));
	o_Color *=  v_TintColor;

	if (o_Color.a < threshhold)
	{
		discard;
	}

	o_Color = o_Color * vec4(v_SunlightIntensity, v_SunlightIntensity, v_SunlightIntensity, 1.0f) ; 
	o_Color.xyz = mix(o_Color.xyz * 1.1f, o_Color.xyz * 1.1 * SkyTint.xyz, 0.65f);
	o_Color = mix(u_FogColor, o_Color, v_Visibility);
	o_Color = vec4(RomBinDaHouseToneMapping(o_Color.xyz), o_Color.w);
}