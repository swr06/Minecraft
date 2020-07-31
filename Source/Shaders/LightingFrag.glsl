#version 330 core
out vec4 o_Color;

in vec3 v_Normal;  
in vec3 v_FragPos;  
  
uniform vec3 u_LightPos; 
uniform vec3 u_ViewPos; 
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

void main()
{
    // ambient
    const float ambient_strength = 0.1;
    const float specular_strength = 0.6;
    const float specular_component = 4;

    vec3 ambient = ambient_strength * u_LightColor;
  
    vec3 norm = normalize(v_Normal);
    vec3 light_dir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_LightColor;
   
    vec3 view_dir = normalize(u_ViewPos - v_FragPos);

    // Normal lighting (Phong)
    //vec3 reflect_dir = reflect(-light_dir, norm);  
    //float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_component);

    // Uses blinn - phong
    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(norm, halfway_dir), 0.0), specular_component);

    vec3 specular = specular_strength * spec * u_LightColor;  
    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;

    // Output the color
    o_Color = vec4(result, 1.0);
} 