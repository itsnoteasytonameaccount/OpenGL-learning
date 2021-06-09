#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

out VS_OUT {
    vec3 color;
} vs_out;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
};

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    gl_PointSize = gl_Position.z;
    TexCoords = aTexCoords;
    vs_out.color = aColor;
}