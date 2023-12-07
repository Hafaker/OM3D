#version 450

layout(binding = 0) uniform sampler2D in_depth;

vec3 unproject(vec2 uv, float depth, mat4 inv_viewproj) {
    const vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
    const vec4 p = inv_viewproj * vec4(ndc, 1.0);
    return p.xyz / p.w;
}

layout(binding = 1) buffer PointLights {
    PointLight point_lights[];
};

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);

    float depth = pow(texelFetch(in_depth, coord).r,0.3);
    if (depth == 0.0)
        return;
    mat4 inv_viewproj;
    vec3 pos = unproject(in_uv, depth, inv_viewproj);

    vec3 acc = vec3(0.0);

    for(uint i = 0; i != frame.point_light_count; ++i) {
        PointLight light = point_lights[i];
        const vec3 to_light = (light.position - in_position);
        const float dist = length(to_light);
        const vec3 light_vec = to_light / dist;

        const float NoL = dot(light_vec, normal);
        const float att = attenuation(dist, light.radius);
        if(NoL <= 0.0 || att <= 0.0f) {
            continue;
        }

        acc += light.color * (NoL * att);
    }
}