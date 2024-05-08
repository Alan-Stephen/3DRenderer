#version 330 core

in vec2 tex;
in vec3 norm;
in vec3 pos;
in vec4 frag_pos_light;

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D shadow_map;
uniform float shininess;
uniform float transparency; 

uniform vec3 cam_pos;

struct SpotLight {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outer_cutoff;
};

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct PointLight {
	vec3 position;   
	vec3 diffuse;    
	vec3 specular;   
	vec3 ambient;    

	float constant;
	float linear;
	float quadratic;
};

#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2
uniform PointLight point_lights[NUM_POINT_LIGHTS];
uniform SpotLight spot_lights[NUM_SPOT_LIGHTS];
uniform DirectionalLight directional_light;

out vec4 fragColour;

vec3 calculate_direct_light(DirectionalLight light, vec3 normal, vec3 view_direction)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), 16); // shininess a bit too high thoguh
    // combine results


    float shadow = 0.0f;
    vec3 light_coords = frag_pos_light.xyz / frag_pos_light.w;

    if(light_coords.z <= 1.0f) 
    {
        light_coords = (light_coords + 1.0f) / 2.0f;

        float closest_depth;
        float current_depth = light_coords.z;

        float bias = max(0.005 * (1 - dot(normal,lightDir)), 0.00005);
        int sampleRadius = 4;

		vec2 pixelSize = 1.0 / textureSize(shadow_map, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        closest_depth = texture(shadow_map, light_coords.xy + vec2(x, y) * pixelSize).r;
				if (current_depth > closest_depth + bias)
					shadow += 1.0f;     
		    }    
		}

		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
    }

	vec3 diffuse_res = vec3(texture(diffuse_tex, tex));
    vec3 ambient  = light.ambient  * diffuse_res;
    vec3 diffuse  = light.diffuse  * diff * diffuse_res * (1 - shadow);
    vec3 specular = light.specular * spec * vec3(texture(specular_tex, tex)) * (1 - shadow);
    return (ambient + diffuse + specular);
}  

vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 frag_position, vec3 view_direction) 
{
    vec3 lightDir = normalize(light.position - frag_position);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), 16); // 16 is shininess
    // attenuation
    float distance    = length(light.position - frag_position);

    // angle 
    float angle = dot(vec3(0.0f,-1.0f, 0.0f), -lightDir);
    float intensity = clamp((angle - light.outer_cutoff) / (light.cutoff - light.outer_cutoff), 0.0f, 1.0f);


	vec3 diffuse_res = vec3(texture(diffuse_tex, tex));
    vec3 ambient  = light.ambient  * diffuse_res;
    vec3 diffuse  = light.diffuse  * diff * diffuse_res * intensity;
    vec3 specular = light.specular * spec * vec3(texture(specular_tex, tex)) * intensity;

    return (ambient + diffuse + specular);
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction) {
    vec3 lightDir = normalize(light.position - frag_position);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), shininess); // 16 is shininess
    // attenuation
    float distance    = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
	vec3 diffuse_res = vec3(texture(diffuse_tex, tex));
    vec3 ambient  = light.ambient  * diffuse_res;
    vec3 diffuse  = light.diffuse  * diff * diffuse_res;
    vec3 specular = light.specular * spec * vec3(texture(specular_tex, tex));

    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
	vec4 col = texture(diffuse_tex,tex);
	float a = col.a;

    // we check to see if transparency is not the default value (1.0f). 
    if(transparency != 1.0f) {
        a = transparency;
    }
	
	if(a == 0.0) {
		discard;
	}
    vec3 normal = normalize(norm);
    vec3 view_direction = normalize(cam_pos - pos);
	
	vec3 result = calculate_direct_light(directional_light, normal, view_direction); 
	for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
		result += calculate_point_light(point_lights[i], normal,pos,view_direction);
	}

    for(int i = 0; i < NUM_SPOT_LIGHTS; i++) {
		result += calculate_spot_light(spot_lights[i], normal,pos,view_direction);
	}
	
	fragColour = vec4(result, a);
}
