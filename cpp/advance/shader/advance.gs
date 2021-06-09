#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
    fColor = gs_in[0].color; // gs_in[0] 因为只有一个输入顶点
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下  
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
    fColor = vec3(1.0f, 1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();  
}

void main()
{
    // gl_Position = gl_in[0].gl_Position + vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    // EmitVertex();

    // gl_Position = gl_in[0].gl_Position + vec4(1.0f, 0.0f, 0.0f, 0.0f);
    // EmitVertex();

    // EndPrimitive();
    build_house(gl_in[0].gl_Position);
}