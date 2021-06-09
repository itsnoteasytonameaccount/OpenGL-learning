#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; 
uniform mat4 normalMatrix;

out vec4 Normal;
out vec4 FragPos;
out vec2 TexCoords;

void main()
{
    FragPos = view * model * vec4(aPos, 1.0f);
    gl_Position = projection * FragPos;
    
    // 乘以法线矩阵后第四个分量会变为不是0
    Normal = vec4((normalMatrix * vec4(aNormal, 0.0f)).xyz, 0.0f);
    TexCoords = aTexCoords;
}