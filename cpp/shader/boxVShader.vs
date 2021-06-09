#version 330 core

layout (location = 0) in vec3 point;
layout (location = 1) in vec2 coord;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
out vec2 texCoord;

void main(){
    gl_Position = projection * view * model * vec4(point, 1);
    texCoord = coord;
}