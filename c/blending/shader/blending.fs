#version 330 core

in vec2 TexCoords;

uniform sampler2D tex;
uniform int is_texture;

out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth)
{
    float z = depth * 2.0f - 1.0f;
    return (2.0f * near * far) / (far + near - z * (far - near));
}

void main()
{
    if(is_texture == 1)
    {
        FragColor = texture(tex, TexCoords);
        // if(FragColor.a < 0.1)
        // {
        //     discard;
        // }
    }
    else
    {
        FragColor = vec4(0.04, 0.28, 0.26, 1.0);
    }
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
    // float depth = (LinearizeDepth(gl_FragCoord.z) - near)/ (far - near);
    // FragColor = vec4(vec3(depth), 1.0f);
}