#version 330 core

in vec2 tex;
in vec3 norm;
in vec3 pos;

uniform sampler2D diffuseTex;
uniform sampler2D specular_tex;

uniform vec3 light_pos;
uniform vec3 light_col;
uniform vec3 cam_pos;

out vec4 fragColour;
vec3 direct_light()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(norm);
	vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - pos);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = specular_amount * specular_light;

	return vec3(texture(diffuseTex, tex)) * (diffuse + ambient) + texture(specular_tex, tex).r * specular * light_col;
}

vec3 spot_light()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(norm);
	vec3 light_direction = normalize(light_pos - pos);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - pos);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return vec3(texture(diffuseTex, tex) * (diffuse * inten + ambient) + texture(specular_tex, tex).r * specular * inten) * light_col;
}

vec3 point_light() {
	float ambient = 0.0;

	vec3 normal = normalize(norm);
	vec3 light_direction = normalize(light_pos - pos);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	float specular_light = 0.5;
	vec3 view_direction = normalize(cam_pos - pos);
	vec3 reflec_direction = reflect(-light_direction, normal);
	float specular_amount = pow(max(dot(view_direction, reflec_direction), 0.0f), 16);
	float specular = specular_amount * specular_light;

	vec4 col = texture(diffuseTex,tex);
	return (vec3(col) * (diffuse + ambient) * light_col + texture(specular_tex, tex).r * specular);
}
void main()
{
	vec4 col = texture(diffuseTex,tex);
	float a = col.a;

	fragColour = vec4(spot_light(),a);
}
