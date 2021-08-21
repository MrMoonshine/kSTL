#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    mat4 MVP = ubo.model * ubo.view * ubo.proj;
    gl_Position = vec4(inPosition, 1.0);
    vec4 tcolor = vec4(0.6, 0.3, 0.7, 0.0);
    //vec4 tcolor = vec4(0.7, 0.5, 0.1, 0.0);
    fragColor = tcolor.xyz;
}
