#version 330 core
in vec3 fragColor;
in vec2 texCoord;
out vec4 FragmentColor;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main(){
    vec2 reverse = vec2(1-texCoord.x, texCoord.y);
    FragmentColor = mix(texture(ourTexture, texCoord),texture(ourTexture2, reverse), 0.2) ;
    // FragmentColor = texture(ourTexture2, texCoord) * vec4(fragColor, 1.0);
}