#version 330 core

uniform sampler2D blurTex;
uniform sampler2D hdrTex;
uniform float exposure;

in vec2 texCoord;

out vec4 color;

void main()
{
    vec3 blurColor = texture(blurTex, texCoord).rgb;
    vec3 hdrColor = texture(hdrTex, texCoord).rgb;
    const float gamma = 2.2f;

    vec3 result = hdrColor + blurColor;
    result = vec3(1.0f) - exp(-result * exposure);
    result = pow(result, vec3(1.0f / gamma));

    color = vec4(result, 1.0f);
}