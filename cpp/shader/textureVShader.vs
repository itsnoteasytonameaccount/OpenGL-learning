#version 330 core
layout (location = 0) in vec3 point;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 aTexCoord;
out vec2 texCoord;
out vec3 fragColor;

// uniform mat4 mytransform;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    // gl_Position = mytransform * vec4(point, 1.0);
    gl_Position = projection * view * model*  vec4(point, 1.0);
    texCoord = aTexCoord;
    fragColor = vColor;
}