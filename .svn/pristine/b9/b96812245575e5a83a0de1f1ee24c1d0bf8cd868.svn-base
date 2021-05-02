#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    // fragColor = texture(texture1,texCoord);
    // fragColor = verc4(texCoord,0,1);
    fragColor = mix(texture(texture1,texCoord),texture(texture2,texCoord),0.3);
}