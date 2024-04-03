#version 330 core

in vec2 tex;
in vec3 norm;
in vec3 pos;

uniform sampler2D diffuseTex;
uniform sampler2D specular_tex;

uniform vec3 light_pos;
uniform vec4 light_col;
uniform vec3 cam_pos;

out vec4 fragColour;

void main()
{
	vec4 col = texture(diffuseTex, tex);

	if(col.a == 0.0) {
		discard;
	}

	vec3 light_vec = (light_pos - pos);
	float dist = length(light_vec);
	float a = 0.000001;
	float b = 0.001;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.1;
	vec3 normal = normalize(norm);

	vec3 light_dir = normalize(light_vec);

	float transparency = col.a;

	float diffuse = max(dot(normal, light_dir), 0.0f);

	float specular_light = 0.5f;
	vec3 view_dir = normalize(cam_pos -	pos);
	vec3 reflection_dir = reflect(-light_dir,normal);

	float specular_amount = pow(max(dot(view_dir,reflection_dir), 0.0f), 8);
	float specular = specular_amount * specular_light;

	col = col * light_col * (diffuse * inten + ambient) + texture(specular_tex, tex).r * specular * inten;
	col.a = transparency;
	fragColour = col;
}
