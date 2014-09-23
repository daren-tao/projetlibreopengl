#version 330

// the input for the position of the vertex
layout(location=0) in vec4 in_Position;

// the input for the texture coords
layout(location=1) in vec2 in_TexCoord;

// the texture out coords
out vec2 texture_coords;

void main(void) {

    // simply passing through the positions for both vertex and texture
    texture_coords = in_TexCoord;
    gl_Position = in_Position;
}
