attribute vec3 vertexPosition;
attribute vec2 textureCoord;

uniform mat4 projectionPosition;
uniform mat4 modelViewPosition;

varying lowp vec4 vColor;
varying highp vec2 vTextureCoord;
void main(){
    gl_Position = projectionPosition * modelViewPosition * vec4(vertexPosition, 1.0);
    // vColor = vertexColor;
    vTextureCoord = textureCoord;
}