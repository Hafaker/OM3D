#version 450

#include "utils.glsl"

layout(location = 0) out vec4 out_color;

layout(location = 0) uniform struct SunLight
{
    vec3 direction;
    vec3 color;
} uSun;

layout(binding = 0) uniform sampler2D in_color;
layout(binding = 1) uniform sampler2D in_normal;
layout(binding = 2) uniform sampler2D in_depth;


const vec3 ambient = vec3(0.0);

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);
    vec3 normal = normalize(texelFetch(in_normal, coord, 0).xyz);
    vec3 color = texelFetch(in_color, coord, 0).xyz;
    float depth = texelFetch(in_depth, coord, 0).x;

    vec3 hdr = uSun.color * max(0.0, dot(uSun.direction, normal)) * color;

    out_color = vec4(hdr, 1.0);
}