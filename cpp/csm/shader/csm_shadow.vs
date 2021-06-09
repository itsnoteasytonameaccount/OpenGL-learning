#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 3) in mat4 model;

uniform mat4 projection_view;
uniform mat4 model2;
// uniform mat4 view;
// uniform mat4 projection;
uniform int use_uniform;

void main()
{
    gl_Position = projection_view * (use_uniform == 1 ? model2 : model) * vec4(aFragPos, 1.0f);
    // gl_Position = projection * view * (use_uniform == 1 ? model2 : model) * vec4(aFragPos, 1.0f);
}