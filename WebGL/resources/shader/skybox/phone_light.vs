#version 300 es
layout (location = 0) in lowp vec3 vertex;
layout (location = 1) in lowp vec3 normals;
layout (location = 2) in lowp vec2 texCroods;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalModel;
out lowp vec3 Normal;
out lowp vec3 FragPos;
out lowp vec2 vTexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(vertex, 1.0f);
    Normal = normalize(normalModel * normals);
    // FragPos = vec3(model * vec4(vertex, 1.0f));
	FragPos = vec3(view * model * vec4(vertex, 1.0f));
    vTexCoords = texCroods;
}