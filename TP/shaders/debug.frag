#version 450

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 in_uv;

layout(binding = 0) uniform sampler2D in_color;
layout(binding = 1) uniform sampler2D in_normal;
layout(binding = 2) uniform sampler2D in_depth;

uniform uint debug_mode;
uniform float exposure = 1.0;


void main() {

    const ivec2 coord = ivec2(gl_FragCoord.xy);

    if (debug_mode == 1)
        out_color = vec4(texelFetch(in_color, coord, 0).rgb, 1.0) * exposure;
    else if (debug_mode == 2)
        out_color = vec4(texelFetch(in_normal, coord, 0).rgb, 1.0) * exposure;
    else if (debug_mode == 3)
        out_color = vec4(pow(texelFetch(in_depth, coord, 0).r, 0.3).xxx, 1.0) * exposure;


    else 
        out_color = texture(in_color, in_uv);

}