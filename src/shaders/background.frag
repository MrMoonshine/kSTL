#version 330

in vec4 color0;
in vec4 color1;
in vec2 uv;

void main(void)
{
    gl_FragColor = mix(color0, color1, smoothstep(0.0, 1.0, uv.y + 1.1));
}
