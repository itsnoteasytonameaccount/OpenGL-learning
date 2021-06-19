#version 330 core

layout (location = 0) in vec2 aFragPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(aFragPos, 0.0f, 1.0f);
    texCoord = aTexCoord;
}