#version 330 core

in vec3 v_TexCoords;
in float v_SunlightIntensity;

uniform samplerCube u_Skybox;

out vec4 o_Color;

void main()
{    
    o_Color = texture(u_Skybox, v_TexCoords);
    o_Color = o_Color * vec4(v_SunlightIntensity, v_SunlightIntensity, v_SunlightIntensity, 1.0f);
}