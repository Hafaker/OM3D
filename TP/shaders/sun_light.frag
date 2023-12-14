#version 450

#include "utils.glsl"

layout(location = 0) out vec4 out_color;

uniform vec3 uSunDirection;
uniform vec3 uSunColor;

layout(binding = 0) uniform sampler2D in_color;
layout(binding = 1) uniform sampler2D in_normal;
layout(binding = 2) uniform sampler2D in_depth;

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);
    vec3 normal = normalize(texelFetch(in_normal, coord, 0).xyz);
    vec3 color = texelFetch(in_color, coord, 0).xyz;
    float depth = texelFetch(in_depth, coord, 0).x;

    vec3 hdr = uSunColor * max(0.0, dot(uSunDirection, normal)) * color;

    out_color = vec4(hdr, 1.0);
}