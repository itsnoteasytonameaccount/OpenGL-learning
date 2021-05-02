#version 330 core

struct Material{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

in vec3 FragPos;
in vec3 Normal;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform Material material;
uniform Light light;
out vec4 FragColor;

void main()
{
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0f);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    FragColor = vec4(ambient + specular + diffuse, 1.0f);
}