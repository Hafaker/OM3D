#version 450

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 in_uv;

layout(binding = 0) uniform sampler2D in_color;
layout(binding = 1) uniform sampler2D in_normal;

layout(binding = 0) uniform Data {
    FrameData frame;
};

const vec3 ambient = vec3(0.0);

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);

    vec3 normal = texelFetch(in_depth, coord);
    vec3 acc = frame.sun_color * max(0.0, dot(frame.sun_dir, normal)) + ambient;

    vec3 color = texelFetch(in_color, coord);
    out_color = vec4(color * acc, 1.0);
}