#version 330 core
in vec2 TexCoords;
in vec4 Normal;
in vec4 FragPos;

struct Light
{
    vec4 vector;
    vec4 direction;
    float cutOff;
    float outerCutOff;

    vec4 ambient;
    vec4 specular;
    vec4 diffuse;
    
    float constant;
    float linear;
    float quadratic;
};

// struct PointLight
// {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;

//     vec3 position;

//     float constant;
//     float linear;
//     float quadratic;
// };

// struct DirLight
// {
//     vec3 direction;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// struct SpotLight
// {
//     vec3 position;
//     vec3 direction;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
    
//     float constant;
//     float linear;
//     float quadratic;
// };

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;
// uniform Light light;
uniform Light spotLight;
uniform Light dirLight;

#define NR_POINT_LGIHTS 4
uniform Light lights[NR_POINT_LGIHTS];

out vec4 FragColor;

vec3 CalcLight(Light light, vec4 normal, vec4 viewDir, vec4 diffuseTexture, vec4 specularTexture)
{
    vec4 ambient = light.ambient * diffuseTexture;

    vec4 lightDir;
    float distance, attenuation = 1.0f;
    if(light.vector.w == 1.0f)
    {
        lightDir = light.vector - FragPos;
        distance = length(lightDir);
        attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance *distance);
        lightDir = normalize(lightDir);
    }
    else
    {
        lightDir = normalize(-light.vector);
    }

    float intensity = 0.0f, theta = dot(light.direction, -lightDir);
    if(theta >= light.cutOff)
    {
        intensity = 1.0f;
    }
    else if(theta >= light.outerCutOff)
    {
        intensity = (theta - light.outerCutOff)/(light.cutOff - light.outerCutOff);
    }

    float diff = max(dot(lightDir, normal), 0.0f);
    vec4 diffuse = light.diffuse * diff * diffuseTexture;

    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec4 specular = light.specular * spec * specularTexture;

    vec3 result = (diffuse + specular).xyz * intensity + ambient.xyz;
    return result.xyz * attenuation;
}

// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseTexture, vec3 specularTexture)
// {
//     vec3 ambient = light.ambient * diffuseTexture;

//     vec3 lightDir = light.position - FragPos;
//     float distance = length(lightDir);
//     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
//     lightDir = normalize(lightDir);
//     float diff = max(dot(lightDir, normal), 0.0f);
//     vec3 diffuse = light.diffuse * diff * diffuseTexture;

//     vec3 reflectDir = reflect(-lightDir, normal); 
//     float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
//     vec3 specular = light.specular * spec * specularTexture;

//     return (ambient + diffuse + specular);
// }

// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseTexture, vec3 specularTexture)
// {
//     vec3 ambient = light.ambient * diffuseTexture;

//     vec3 lightDir = normalize(-light.direction);
//     float diff = max(dot(lightDir, normal), 0.0f);
//     vec3 diffuse = light.diffuseTexture * diff * diffuseTexture;

//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
//     vec3 specular = light.specular * spec * specularTexture;

//     return (ambient + diffuse + specular);
// }

// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3  viewDir, vec3 diffuseTexture, vec3 specularTexture)
// {
//     vec3 ambient = light.ambient * diffuseTexture;

//     vec3 lightDir = light.position - FragPos;
//     float distance = length(lightDir);
//     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
//     lightDir = normalize(lightDir);
//     float diff = max(dot(lightDir, normal), 0.0f);
//     vec3 diffuse = light.diffuse * diff * diffuseTexture;

//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(reflectDir, normal), 0.0f), material.shininess);
//     vec3 specular = light.specular * spec * specularTexture;

//     float theta = dot(-lightDir, light.direction);
//     float intensity = clamp((theta - light.outerCutOff)/(light.cutOff - light.outerCutOff), 0.0f, 1.0f);

//     return (ambient + (diffuse + specular) * intensity) * attenuation;
// }

void main()
{
    vec4 diffuseTexture = texture(material.diffuse, TexCoords);
    vec4 specularTexture = texture(material.specular, TexCoords);
    vec4 viewDir = normalize(vec4(0.0f, 0.0f, 0.0f, 1.0f) - FragPos);
    vec3 result = CalcLight(dirLight, Normal, viewDir, diffuseTexture, specularTexture);
    result += CalcLight(spotLight, Normal, viewDir, diffuseTexture, specularTexture);
    for(int i = 0; i < NR_POINT_LGIHTS; i++)
    {
        result += CalcLight(lights[i], Normal, viewDir, diffuseTexture, specularTexture);
    }

    FragColor = vec4(result, 1.0f);
}