#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 filament;

out vec3 normal;
out vec3 fragPos;
out vec3 fragColor;

void main() {
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    
    fragPos = inPosition;
    normal = inNormals;
    fragColor = filament;
}
