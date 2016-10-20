#version 330 core

//Structs
struct DirLight {
    vec3 direction;

	vec3 color;
};  

struct PointLight {
    vec3 position;
    
    float quadratic;
	
	vec3 color;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;

	if(lightType == 0)
		result = CalcDirLight(dirLight, norm, viewDir);
	else if(lightType == 1)
		result = CalcPointLight(pointLight, norm, FragPos, viewDir);
	//else if(lightType == 2)
	color = vec4(result, 1.0f);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//Direction
	vec3 lightDir = normalize(-light.direction);
	
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * material.diffuse;

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular;

	//ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * material.ambient;

    return (diffuse + specular + ambient) * light.color;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Direction
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * material.diffuse;

	// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular;

	//ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * material.ambient;

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.quadratic * (distance * distance));

	return (diffuse + specular + ambient) * light.color * attenuation;
}
