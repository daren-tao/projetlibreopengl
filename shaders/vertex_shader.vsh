#version 330

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_TexCoord;

out vec2 texture_coords;

void main(void) {
    texture_coords = in_TexCoord;
    gl_Position = in_Position;
}
