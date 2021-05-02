#version 330 core
layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;

void main()
{
    FragPos = aFragPos;
    gl_Position = (projection * view * vec4(aFragPos, 1.0f)).xyww;
}