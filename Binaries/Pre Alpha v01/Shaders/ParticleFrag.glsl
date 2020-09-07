#version 330 core

in vec2 v_TexCoords;
uniform sampler2D u_Texture;

out vec4 o_Color;

void main()
{
	o_Color = texelFetch(u_Texture, ivec2(v_TexCoords.xy), 0);

	if (o_Color.a < 0.1f)
	{
		discard;
	}
}