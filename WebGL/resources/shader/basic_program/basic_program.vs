#version 300 es

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

out lowp vec4 vColor;
out highp vec2 vTextureCoord;

void main(){
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1.0);
    vTextureCoord = textureCoord;
}