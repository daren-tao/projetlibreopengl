#version 330

uniform sampler2D image_texture;

in vec2 texture_coords;

out vec4 out_Color;

void main(void) {
    out_Color = texture2D(image_texture, texture_coords);
}
