#version 330

layout(location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 fragColor;

void main() {
    //mat4 MVP = ubo.model * ubo.view * ubo.proj;
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    vec3 tcolor = normalize(inPosition);
    
    fragColor = tcolor.xyz;
}
