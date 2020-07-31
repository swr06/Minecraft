#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal; 
    v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0f));
}