#version 330 core
in float v_Visibility;
in vec2 v_TexCoord;
in vec4 v_TintColor;
in float v_SunlightIntensity;

in vec3 v_WorldPosition;

flat in int v_LightlevelRAW;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec4 u_FogColor;

uniform float u_Time;

uniform int u_Transparent;

vec3 SkyTint = vec3(151.0f, 183.0f, 245.0f) / 255.0f;

bool CompareVec3(vec3 v1, vec3 v2) {
	float e = 0.01f;
	return abs(v1.x - v2.x) < e && abs(v1.y - v2.y) < e && abs(v1.z - v2.z) < e;
}

void CalculateUV(vec3 world_pos, in vec3 normal, out vec2 uv)
{
	const vec3 NORMAL_TOP = vec3(0.0f, 1.0f, 0.0f);
	const vec3 NORMAL_BOTTOM = vec3(0.0f, -1.0f, 0.0f);
	const vec3 NORMAL_FRONT = vec3(0.0f, 0.0f, 1.0f);
	const vec3 NORMAL_BACK = vec3(0.0f, 0.0f, -1.0f);
	const vec3 NORMAL_LEFT = vec3(-1.0f, 0.0f, 0.0f);
	const vec3 NORMAL_RIGHT = vec3(1.0f, 0.0f, 0.0f);

    if (CompareVec3(normal, NORMAL_TOP))
    {
        uv = vec2(fract(world_pos.xz));
    }

    else if (CompareVec3(normal, NORMAL_BOTTOM))
    {
        uv = vec2(fract(world_pos.xz));
    }

    else if (CompareVec3(normal, NORMAL_RIGHT))
    {
        uv = vec2(fract(world_pos.zy));
    }

    else if (CompareVec3(normal, NORMAL_LEFT))
    {
        uv = vec2(fract(world_pos.zy));
    }
    
    else if (CompareVec3(normal, NORMAL_FRONT))
    {
        uv = vec2(fract(world_pos.xy));
    }

     else if (CompareVec3(normal, NORMAL_BACK))
    {
        uv = vec2(fract(world_pos.xy));
    }
}




vec3 RomBinDaHouseToneMapping(vec3 color)
{
    color = exp( -1.0 / ( 2.72*color + 0.15 ) );
	color = pow(color, vec3(1. / 2.5));
	return color;
}

vec4 GetWater(vec2 puv) {
	vec4 texture_color = vec4(0.192156862745098, 0.6627450980392157, 0.9333333333333333, 1.0);
    vec4 k = vec4(u_Time)*0.8;
	k.xy = puv.xy * 7.0;
    float val1 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
    float val2 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.2));
    float val3 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
    vec4 color = vec4 ( pow(min(min(val1,val2),val3), 5.0) * 3.0)+texture_color;
	return color;
}

vec2 BasicTextureDistortion(vec2 UV) {
    vec2 UVxy = UV.xy;
    float time = u_Time;
    UV.x += sin(time * 0.25f);
    UV.y += pow(cos(time * 0.15f),2.);
    UV.x += cos(UV.x*10.0f + time)*0.3f;
    UV.y += sin(UV.y*5.0f + UV.x*4.0f + time*1.3f)*0.4f;
    UV.xy = mix(UV.xy,UVxy.xy,0.91f);
    return UV;
}

vec4 PixelArtFilter(sampler2D tex, vec2 uv)
{
    vec2 res = vec2(textureSize(tex,0));
    uv = uv*res;
    vec2 seam = floor(uv+0.5);
    uv = seam + clamp( (uv-seam)/fwidth(uv), -0.5, 0.5);
    return texture(tex, uv/res);
}

vec2 HalfCorrection(int x, int y, int tex_width, int tex_height) {
    float u = (x + 0.5) / tex_width;
    float v = (y + 0.5) / tex_height;
    return vec2(u, v);
 }

void main()
{
	const float threshhold = 0.1;
	o_Color =  texelFetch(u_Texture, ivec2(v_TexCoord.xy), 0) ;
	//o_Color =  PixelArtFilter(u_Texture, vec2(HalfCorrection(int(v_TexCoord.x), int(v_TexCoord.y), 3048, 64))) ;
	
	o_Color.xyz = pow(o_Color.xyz, vec3(2.2));
	o_Color *=  v_TintColor;

	if (o_Color.a < threshhold)
	{
		discard;
	}

	o_Color = o_Color * vec4(v_SunlightIntensity, v_SunlightIntensity, v_SunlightIntensity, 1.0f) ; 
	o_Color.xyz = mix(o_Color.xyz * 1.1f, o_Color.xyz * 1.1 * SkyTint.xyz, 0.65f);


     // water 
	if (v_LightlevelRAW > 84 && u_Transparent == 1) {
	}

	o_Color = mix(vec4(pow(u_FogColor.xyz, vec3(1.75)), u_FogColor.w), o_Color, v_Visibility);
	o_Color = vec4(RomBinDaHouseToneMapping(o_Color.xyz), o_Color.w);

}