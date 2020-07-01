#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_TexCoords = a_Position;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}  