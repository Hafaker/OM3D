#version 450

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 in_uv;

layout(binding = 0) uniform sampler2D in_color;
layout(binding = 1) uniform sampler2D in_normal;
layout(binding = 2) uniform sampler2D in_depth;

uniform uint debug_mode;

void main() {
    if (debug_mode == 1)
        out_color = texture(in_color, in_uv);
    else if (debug_mode == 2)
        out_color = texture(in_normal, in_uv);
    else if (debug_mode == 3)
        out_color = pow(texture(in_depth, in_uv).r,0.3).xxxx;

    else 
        out_color = texture(in_color, in_uv);

}