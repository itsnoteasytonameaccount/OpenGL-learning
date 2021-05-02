#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2DMS texms;
uniform int effect;

#define INVERSION 0
#define GRAYSCALE 1
#define BLUR 2
#define SHARPEN 3
#define EDGE_DETECTION 4
#define CUSTOM_ANTI_ALIAS 5

const float offset = 1.0f / 300.0f;

vec2[] offsets = vec2[](
    vec2(-offset, offset),
    vec2(-offset, 0),
    vec2(-offset, -offset),
    vec2(offset, offset),
    vec2(offset, 0),
    vec2(offset, -offset),
    vec2(0, offset),
    vec2(0, 0),
    vec2(0, -offset)
);

vec4 Inversion()
{
    return vec4(1.0f - texture(tex, TexCoords).xyz, 1.0f);
}

vec4 Grayscale()
{
    vec4 result = texture(tex, TexCoords);
    float average = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
    return vec4(average, average, average, 1.0f);
}

vec4 KernelEffect(float kernel[9])
{
    vec3 col = vec3(0.0f);
    for(int i = 0; i < 9; i++)
    {
        col += kernel[i] * texture(tex, TexCoords + offsets[i]).xyz;
    }
    return vec4(col, 1.0f);
}

void main()
{
    ivec2 texSize = textureSize(texms);
    switch(effect)
    {
    case INVERSION:
        FragColor = Inversion();
        break;
    case GRAYSCALE:
        FragColor = Grayscale();
        break;
    case BLUR:
        FragColor = KernelEffect(float[](
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16  
        ));
        break;
    case SHARPEN:
        FragColor = KernelEffect(float[](
            -1, -1, -1,
            -1,  9, -1,
            -1, -1, -1
        ));
        break;
    case EDGE_DETECTION:
        FragColor = KernelEffect(float[](
             1,  1,  1,
             1, -8,  1,
             1,  1,  1
        ));
        break;
    case CUSTOM_ANTI_ALIAS:
        FragColor = vec4(0.0f);
        FragColor += texelFetch(texms, ivec2(TexCoords * texSize), 0)/4;
        FragColor += texelFetch(texms, ivec2(TexCoords * texSize), 1)/4;
        FragColor += texelFetch(texms, ivec2(TexCoords * texSize), 2)/4;
        FragColor += texelFetch(texms, ivec2(TexCoords * texSize), 3)/4;
        break;
    default:
        FragColor = texture(tex, TexCoords);
        break;
    }
}