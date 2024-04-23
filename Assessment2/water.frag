#version 330 core

in vec3 pos;
in vec3 norm;

uniform vec3 light_pos;
uniform vec3 light_col;
uniform vec3 cam_pos;

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directional_light;

vec3 calculate_direct_light(DirectionalLight light, vec3 normal, vec3 view_direction)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), 32); // shininess a bit too high thoguh
    // combine results

	vec3 diffuse_res = vec3(0.1,0.5,1.0);
    vec3 ambient  = light.ambient  * diffuse_res;
    vec3 diffuse  = light.diffuse  * diff * diffuse_res;
    vec3 specular = light.specular * spec * vec3(1.0,1.0,1.0);
    return (ambient + diffuse + specular);
}  

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(norm);
    vec3 view_direction = normalize(cam_pos - pos);

	vec3 result = calculate_direct_light(directional_light, normal, view_direction); 
    fragColor = vec4(result, 1.0);
}
