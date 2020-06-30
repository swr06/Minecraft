#version 330 core

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

out vec4 color;

void main()
{    
    //Color = texture(u_Skybox, v_TexCoords);
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}