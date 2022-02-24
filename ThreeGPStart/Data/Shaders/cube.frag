#version 330

uniform vec3 camera_posisition;

uniform vec3 ambient_light;

struct DirectionalLight{
	vec3 direction;
	vec3 colour;
	float intensity;
};
const int MAX_DIRECTIONAL_LIGHTS = 10;
uniform int num_directional_lights;
uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHTS];

struct PointLight{
	vec3 position;
	vec3 colour;
	float intensity;
	float range;
};
const int MAX_POINT_LIGHTS = 10;
uniform int num_point_lights;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

struct SpotLight{
	PointLight base;
	vec3 direction;
	float cosangle;
};
const int MAX_SPOT_LIGHTS = 10;
uniform int num_spot_lights;
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

in vec3 varying_position;
in vec3 varying_normal;
in vec3 varying_colour;

out vec4 fragment_colour;

vec3 CalculateDirectionalLight(DirectionalLight directional_light, vec3 normal)
{
	float diffuse_intensity = max(0, dot(normalize(-directional_light.direction), normal));
	return directional_light.intensity * diffuse_intensity * directional_light.colour;
}

vec3 CalculatePointLight(PointLight point_light, vec3 normal)
{
	vec3 L = normalize(point_light.position - varying_position);

	// DIFFUSE
	float diffuse_intensity = max(0, dot(L, normal));
	float light_distance = distance(point_light.position, varying_position);
	float attenuation = 1.0 - smoothstep(0, point_light.range, light_distance);

	vec3 diffuse = attenuation * diffuse_intensity * point_light.colour;

	// SPECULAR
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(camera_posisition - varying_position);
	vec3 reflectDir = reflect(-L, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0), 32);

	vec3 specular = specularStrength * attenuation * spec * point_light.colour;  

	return vec3(diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight spot_light, vec3 normal)
{
	vec3 L = normalize(spot_light.base.position - varying_position);
	float varyingcos = dot(L, normalize(spot_light.direction));

	if(varyingcos > spot_light.cosangle)
	{
		vec3 colour = CalculatePointLight(spot_light.base, normal);
		float intensity = 1.0f - (1.0f - varyingcos) / (1.0f - spot_light.cosangle);

		return colour * intensity;
	}
	else
	{
		return vec3(0,0,0);
	}
}

void main(void)
{
	vec3 N = normalize(varying_normal); 
	vec3 Result = ambient_light;
	
	for(int i = 0; i < num_directional_lights; i++)
	{
		Result = Result + CalculateDirectionalLight(directional_lights[i], N);
	}

	for(int i = 0; i < num_point_lights; i++)
	{
		Result = Result + CalculatePointLight(point_lights[i], N);
	}

	for(int i = 0; i < num_spot_lights; i++)
	{
		Result = Result + CalculateSpotLight(spot_lights[i], N);
	}
	
	Result = Result * varying_colour;

	fragment_colour = vec4(Result, 1.0);
	//fragment_colour = vec4(varying_normal, 1.0);
}