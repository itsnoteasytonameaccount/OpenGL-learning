#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec4 normal;
} gs_in[];

out vec2 TexCoords;
out vec4 Normal;
uniform float time;

vec3 GetNormal()
{
    vec3 vector1 = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 vector2 = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(vector1, vector2));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.3;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

void main()
{
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    Normal = gs_in[0].normal;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    Normal = gs_in[1].normal;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    Normal = gs_in[2].normal;
    EmitVertex();

    EndPrimitive();
}