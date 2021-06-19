#version 330 core

in vec2 texCoord;
uniform sampler2D tex;
uniform int mode;
uniform float exposure;

out vec4 color;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(tex, texCoord).rgb;
    vec3 mapped;
    switch(mode)
    {
        case 0:
        default:
            mapped = hdrColor;
            break;
        case 1:
            mapped = hdrColor / (hdrColor + vec3(1.0f));
            break;
        case 2:
            mapped = vec3(1.0) - exp(-hdrColor * exposure);
            break;
    }
    mapped = pow(mapped, vec3(1.0f / gamma));
    
    color = vec4(mapped, 1.0f);
}