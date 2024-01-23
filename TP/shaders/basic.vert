#version 450

#include "utils.glsl"

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_tangent_bitangent_sign;
layout(location = 4) in vec3 in_color;
layout(location = 5) in vec4 a_joint;
layout(location = 6) in vec4 a_weight;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_uv;
layout(location = 2) out vec3 out_color;
layout(location = 3) out vec3 out_position;
layout(location = 4) out vec3 out_tangent;
layout(location = 5) out vec3 out_bitangent;

layout(binding = 0) uniform Data {
    FrameData frame;
};

uniform mat4 model;

uniform mat4 u_jointMat0;
uniform mat4 u_jointMat1;

void main() {
    vec4 position = vec4(in_pos, 1.0);

    out_normal = normalize(mat3(model) * in_normal);
    out_tangent = normalize(mat3(model) * in_tangent_bitangent_sign.xyz);
    out_bitangent = cross(out_tangent, out_normal) * (in_tangent_bitangent_sign.w > 0.0 ? 1.0 : -1.0);

    out_uv = in_uv;
    out_color = in_color;
    out_position = position.xyz;

    mat4 u_jointMat[2] = {u_jointMat0, u_jointMat1};


    if (u_jointMat0 != mat4(0.0f)) {
        mat4 skinMat =
            a_weight.x * u_jointMat[int(a_joint.x)] +
            a_weight.y * u_jointMat[int(a_joint.y)] +
            a_weight.z * u_jointMat[int(a_joint.z)] +
            a_weight.w * u_jointMat[int(a_joint.w)];

        vec4 worldPosition = skinMat * vec4(in_pos,1.0);
        vec4 cameraPosition = frame.camera.view * worldPosition;
        gl_Position = frame.camera.proj * cameraPosition;
    }
    else {
        position = model * position;
        gl_Position = frame.camera.view_proj * position;
    }
}

