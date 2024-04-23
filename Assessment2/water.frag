#version 330 core

in vec3 pos;
in vec3 norm;

uniform vec3 light_pos;
uniform vec3 light_col;
uniform vec3 cam_pos;

out vec4 fragColor;

void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light_col;
  	
    // diffuse 
    vec3 norm = normalize(norm);
    vec3 lightDir = normalize(light_pos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_col;
    
    // specular
    float specularStrength = 1.f;
    vec3 viewDir = normalize(cam_pos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * light_col;  
        
    vec3 result = (ambient + diffuse + specular) * vec3(0.1,0.5,1.0);
    fragColor = vec4(result, 1.0);
}
