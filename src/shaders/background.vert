#version 330

layout(location = 0) in vec4 bgvtx;

uniform vec4 mainColor;
uniform vec4 secColor;

out vec4 color0;
out vec4 color1;
out vec2 uv;

void main() {
    //mat4 MVP = ubo.model * ubo.view * ubo.proj;
    gl_Position = bgvtx;
    uv = bgvtx.xy;
    color0 = mainColor;
    color1 = secColor;
}
