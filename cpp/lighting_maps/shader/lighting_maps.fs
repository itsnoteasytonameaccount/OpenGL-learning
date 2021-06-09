#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform sampler2D emissionMaps;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec *  vec3(texture(material.specular, TexCoords));
    // vec3 specular = light.specular * spec *  (1 - vec3(texture(material.specular, TexCoords)));

    vec3 emission = vec3(1.0f) *  vec3(texture(emissionMaps, TexCoords));

    FragColor = vec4(specular + diffuse + ambient + emission, 1.0f);    
}
