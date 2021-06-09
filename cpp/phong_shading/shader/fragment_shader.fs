#version 330 core
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
// uniform vec3 viewPos;
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(-FragPos);
    // vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = Normal;
    vec3 reflectDir = reflect(-lightDir, norm);
    float amibient = 0.2f;
    float specular_strength = 0.9f;
    float diff = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = diff * lightColor;
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 1);
    vec3 specular = specular_strength * spec * lightColor;
    vec3 result = (amibient + specular + diffuse) * objectColor;
    color = vec4(result, 1.0f);
}