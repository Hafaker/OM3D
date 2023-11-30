#version 450

layout(binding = 0) uniform sampler2D in_deph;
layout(binding = 1) uniform sampler2D in_color;
layout(binding = 2) uniform sampler2D in_normal;

layout(location = 0) in vec2 in_uv;

//uniform ...

layout(location = 0) out vec4 out_color;


void main() {
    //if ...
    out_color = texture(in_normal, in_uv);
}