#version 300 es

layout (location = 0) in highp vec3 aPos;
layout (location = 1) in lowp vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main()
{
    gl_Position = (projection * view * vec4(aPos, 1.0f)).xyww;
    TexCoords = aPos;
}
