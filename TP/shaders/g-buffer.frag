#version 450

layout(location = 2) in vec3 in_color;
layout(location = 0) in vec3 in_normal;

layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;

void main()
{
    out_albedo = vec4(in_color, 1.0f);
    out_normals = vec4(in_normal, 1.0f);
}