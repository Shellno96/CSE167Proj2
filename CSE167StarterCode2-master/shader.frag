#version 330 core

//Structs
struct DirLight {
    vec3 direction;

	vec3 diffuse;
	vec3 specular;
	vec3 ambient;

};  

struct PointLight {
    vec3 position;
    
    float quadratic;
	
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
  
    float spotExponent;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
}; 

//Inputs  
in vec3 Normal;
in vec3 FragPos;

//Outputs
out vec4 color;

//Uniforms
uniform int lightType;
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;

	if(lightType == 0)
		result = CalcDirLight(dirLight, norm, viewDir);
	else if(lightType == 1)
		result = CalcPointLight(pointLight, norm, FragPos, viewDir);
	else if(lightType == 2)
		result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
	else if(lightType == 3)
		result = norm;
	color = vec4(result, 1.0f);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//Direction
	vec3 lightDir = normalize(-light.direction);
	
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * material.diffuse * light.diffuse;
	
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular * light.specular;
	
	//ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * material.ambient * light.ambient;

    return (diffuse + specular + ambient);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Direction
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * material.diffuse * light.diffuse;

	// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular * light.specular;

	//ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * material.ambient * light.ambient;

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.quadratic * (distance * distance));

	return (diffuse + specular + ambient) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Direction
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	if(theta > light.cutOff) 
	{       
		 // Do lighting calculations

		 // Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * material.diffuse * light.diffuse;
			
		// Specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = spec * material.specular * light.specular;

		//ambient
		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * material.ambient * light.ambient;

		// Color
		float lightColor = pow(theta, light.spotExponent);

		return (diffuse + specular + ambient) * lightColor;
	}
	else  // else, use ambient light so scene isn't completely dark outside the spotlight.
	{
		//ambient
		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * material.ambient;

		return ambient * light.ambient;
	}
}