#version 300 es
uniform lowp vec3 lightColor;
uniform lowp vec3 lightPos;

uniform sampler2D uSampler;

in lowp vec3 Normal;
in lowp vec3 FragPos;
in lowp vec2 vTexCoords;
out lowp vec4 color;

void main()
{
    lowp vec3 objectColor = texture(uSampler, vec2(vTexCoords.s, vTexCoords.t)).xyz;
    lowp vec3 lightDir = normalize(lightPos - FragPos);
    lowp vec3 viewDir = normalize(-FragPos);
    // lowp vec3 viewDir = normalize(viewPos - FragPos);
    lowp vec3 norm = Normal;
    lowp vec3 reflectDir = reflect(-lightDir, norm);
    lowp float amibient = 0.2f;
    lowp float specular_strength = 0.9f;
    lowp float diff = max(dot(lightDir, norm), 0.0f);
    lowp vec3 diffuse = diff * lightColor;
    lowp float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 1.0f);
    lowp vec3 specular = specular_strength * spec * lightColor;
    lowp vec3 result = (amibient + specular + diffuse) * objectColor;
    color = vec4(result, 1.0f);
}