#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    //Multiply the Transformation matrix into the Position
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //Passing on model position
    fragPos = vec3(model * vec4(aPos, 1.0));
    //Pass normalvectors on to the Fragment shader  
    normal = aNormal;
}