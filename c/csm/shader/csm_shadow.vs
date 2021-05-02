#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 3) in mat4 model;

// uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aFragPos, 1.0f);
}