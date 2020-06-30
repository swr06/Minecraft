#version 330 core

out vec4 Color;
in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    Color = texture(u_Skybox, v_TexCoords);
}