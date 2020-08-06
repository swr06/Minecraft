#version 330 core
in float v_Visibility;
in vec2 v_TexCoord;
in vec4 v_TintColor;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec4 u_FogColor;

void main()
{
	o_Color = texture(u_Texture, v_TexCoord) * v_TintColor ; 
	o_Color = mix(u_FogColor, o_Color, v_Visibility);
}