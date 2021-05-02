#version 330 core

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 TexCoords;
in vec3 fColor;

void main()
{
    // FragColor = vec4(1.0f, 0.3f, 0.3f, 1.0f);
    // if(gl_FragCoord.x < 400)
    //     FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // else
    //     FragColor = vec4(0.0, 1.0, 0.0, 1.0);   
    // if(gl_FrontFacing)
    // {
    //     FragColor = texture(texture1, TexCoords);
    // }
    // else
    // {
    //     FragColor = texture(texture2, TexCoords);
    // }
    // FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    FragColor = vec4(fColor, 1.0f);
}