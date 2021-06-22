#version 330 core

out vec4 color;

uniform bool horizontal;
uniform sampler2D tex;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

in vec2 texCoord;

void main()
{
    vec2 offset = 1.0f / textureSize(tex, 0);
    vec3 result = texture(tex, texCoord).rgb * weight[0];
    if(horizontal)
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(tex, texCoord + vec2(offset.x * i, 0.0f)).rgb * weight[i];
            result += texture(tex, texCoord - vec2(offset.x * i, 0.0f)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(tex, texCoord + vec2(0.0f, offset.y * i)).rgb * weight[i];
            result += texture(tex, texCoord - vec2(0.0f, offset.y * i)).rgb * weight[i];
        }
    }
    color = vec4(result, 1.0f);
}