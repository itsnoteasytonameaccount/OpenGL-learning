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

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
    
    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    int has_texture;
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

uniform Material material;
// uniform Light light;
uniform Light spotLight;
uniform Light dirLight;
uniform Light pointLight;

#define NR_POINT_LGIHTS 4
uniform Light lights[NR_POINT_LGIHTS];

out vec4 FragColor;

vec3 CalcLight(Light light, vec4 normal, vec4 viewDir, vec3 diffuseTexture, vec3 specularTexture, vec3 ambientTexture)
{
    vec3 ambient = light.ambient * ambientTexture;

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
    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

    vec3 specular = light.specular * spec * specularTexture;

    vec3 result = (specular + diffuse) * intensity + ambient;
    return result * attenuation;
}


void main()
{
    vec4 normal = Normal;
    vec3 diffuseTexture;
    vec3 specularTexture;
    vec3 ambientTexture;
    if(material.has_texture == 1)
    {
        specularTexture = vec3(texture(material.texture_specular1, TexCoords));
        diffuseTexture = vec3(texture(material.texture_diffuse1, TexCoords));
    }
    else
    {
        specularTexture = material.specular;
        diffuseTexture = material.diffuse;
        ambientTexture = material.ambient;
    }

    vec4 viewDir = normalize(vec4(0.0f, 0.0f, 0.0f, 1.0f) - FragPos);
    vec3 result;
    result = CalcLight(dirLight, normal, viewDir, diffuseTexture, specularTexture, diffuseTexture);
    result += CalcLight(pointLight, normal, viewDir, diffuseTexture, specularTexture, diffuseTexture);
    result += CalcLight(spotLight, normal, viewDir, diffuseTexture, specularTexture, ambientTexture);

    FragColor = vec4(result, 1.0f);
}