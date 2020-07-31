#version 330 core
out vec4 FragColor;
  
uniform vec3 u_ObjectColor;
uniform vec3 u_DiffuseLightColor;
uniform vec3 u_DiffuseLightPosition;
uniform vec3 u_ViewerPosition;

in vec3 v_Normal;
in vec3 v_FragPos;

void main()
{
    const float ambient_strength = 0.2f;
    const float specular_strength = 0.5f;

    vec3 normal = normalize(v_Normal);
    vec3 light_dir = normalize(u_DiffuseLightPosition - v_FragPos);
    vec3 ambient = u_DiffuseLightColor * ambient_strength;

    vec3 view_direction = normalize(u_ViewerPosition - v_FragPos);
    vec3 reflect_direction = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 1.0f);
    vec3 specular = specular_strength * spec * u_DiffuseLightColor;  

    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = diff * u_DiffuseLightColor;
    vec3 result = diffuse + ambient + specular;

    FragColor = vec4(result * u_ObjectColor, 1.0);
}