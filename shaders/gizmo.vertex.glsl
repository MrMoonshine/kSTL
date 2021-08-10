#version 330

layout (location = 0) in vec3 aPos;

out vec3 fragPos;
uniform mat4 model;

void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));
}