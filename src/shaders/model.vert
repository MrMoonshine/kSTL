#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 filament;
uniform vec3 eye;

out vec3 normal;
out vec3 eyePos;
out vec3 fragPos;
out vec3 fragColor;

void main() {
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    
    eyePos = eye;
    fragPos = inPosition;
    //normal = inNormals;
    normal = (model * vec4(inNormals, 1)).xyz;
    fragColor = filament;
}
