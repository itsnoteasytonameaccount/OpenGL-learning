#version 330 core

in vec3 FragPos;

uniform samplerCube cube;
out vec4 FragColor;

void main()
{
    // float depth = texture(cube, FragPos).r;
    float depth = texture(cube, FragPos).r;
    FragColor = vec4(vec3(depth), 1.0f);
    // FragColor = texture(cube, FragPos);
}
