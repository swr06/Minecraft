#version 330 core
in vec2 v_TexCoord;
in vec4 v_TintColor;

out vec4 o_Color;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, v_TexCoord) * v_TintColor ; 
}