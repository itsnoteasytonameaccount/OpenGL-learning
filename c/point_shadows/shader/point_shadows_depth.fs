#version 330 core

in vec4 FragPos;

uniform float far_plane;
uniform vec3 lightPos;

void main()
{
    float light_distance = length(FragPos.xyz - lightPos);

    light_distance = light_distance / far_plane;

    gl_FragDepth = light_distance;
}