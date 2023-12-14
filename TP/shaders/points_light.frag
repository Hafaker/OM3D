#version 450

#include "utils.glsl"

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 in_uv;

layout(binding = 2) uniform sampler2D in_color;
layout(binding = 3) uniform sampler2D in_depth;
layout(binding = 4) uniform sampler2D in_normal;


uniform mat4 view_proj;
uniform uint point_light_count;

uniform vec3 lightpos[100];
uniform vec3 lightcolor[100];
uniform float lightradius[100];

vec3 unproject(vec2 uv, float depth, mat4 inv_viewproj) {
    const vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
    const vec4 p = inv_viewproj * vec4(ndc, 1.0);
    return p.xyz / p.w;
}

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);
    
    vec3 color = texelFetch(in_color, coord, 0).rgb;
    vec3 normal = texelFetch(in_normal, coord, 0).rgb;
    float depth = pow(texelFetch(in_depth, coord, 0).r,0.3);
    if (depth == 0.0)
        return;

    mat4 inv_viewproj = inverse(view_proj);
    vec3 pos = unproject(in_uv, depth, inv_viewproj);

    vec3 acc = vec3(0.0);

    for (uint i = 0; i != point_light_count; ++i) {
        const vec3 to_light = (lightpos[i] - pos);
        const float dist = length(to_light);
        const vec3 light_vec = to_light / dist;

        const float NoL = dot(light_vec, normal);
        const float att = attenuation(dist, lightradius[i]);
        if(NoL <= 0.0 || att <= 0.0f) {
            continue;
        }
        acc += lightcolor[i] * (NoL * att);
    }

    out_color = vec4(color * acc, 1.0);
}