#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, TexCoord); 
}