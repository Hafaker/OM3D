#version 450

#include "utils.glsl"

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 in_uv;

layout(binding = 2) uniform sampler2D in_color;
layout(binding = 3) uniform sampler2D in_normal;

layout(binding = 0) uniform Data {
    FrameData frame;
};

const vec3 ambient = vec3(0.0);

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);

    vec3 normal = texelFetch(in_normal, coord, 0).rgb;
    vec3 acc = frame.sun_color * max(0.0, dot(frame.sun_dir, normal)) + ambient;

    vec3 color = texelFetch(in_color, coord, 0).rgb;
    out_color = vec4(color * acc, 1.0);
}