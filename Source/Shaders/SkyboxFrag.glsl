#version 330 core

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

out vec4 color;

void main()
{    
    color = texture(u_Skybox, v_TexCoords);
}