#version 330

// the original image's texture
uniform sampler2D image_texture;

// the texture's coords
in vec2 texture_coords;

// the pixel's out color rgba
out vec4 out_Color;

void main(void) {

    // the pixel's color is the color of the texture in these coords
    out_Color = texture2D(image_texture, texture_coords);
}
