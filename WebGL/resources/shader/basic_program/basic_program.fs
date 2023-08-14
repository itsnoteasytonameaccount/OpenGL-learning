#version 300 es

in lowp vec4 vColor;
in highp vec2 vTextureCoord;

uniform sampler2D uSampler;

out highp vec4 outputFragColor;

void main() {
    // outputFragColor = vec4(1.0, 0.0, 0.0, 1.0);
    outputFragColor = texture(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
}