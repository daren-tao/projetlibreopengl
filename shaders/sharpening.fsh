#version 330

// the original image's texture
uniform sampler2D image_texture;

// the offset in x coord
uniform float x_offset;

// the offset in y coord
uniform float y_offset;

// the scale factor used to increase the sharp effect
uniform float scale_factor;

// the array containing all the kernel values
uniform float kernel_value[9];

// the texture's coords
in vec2 texture_coords;

// the pixel's out color rgba
out vec4 out_Color;

void main(void) {

    // the pixel's color is the color of the texture in these coords
    vec4 original = texture2D(image_texture, texture_coords);

    // temporary vec4 used to contain the sum of the neighbors' color
    vec4 temp = vec4(0.0);

    // summing the value of the pixel's color located in (x, y) times the kernel value
    temp += texture2D(image_texture, texture_coords + vec2(-x_offset, y_offset))*kernel_value[0];
    temp += texture2D(image_texture, texture_coords + vec2(0.0, +y_offset))*kernel_value[1];
    temp += texture2D(image_texture, texture_coords + vec2(x_offset, y_offset))*kernel_value[2];
    temp += texture2D(image_texture, texture_coords + vec2(-x_offset, 0.0))*kernel_value[3];
    temp += texture2D(image_texture, texture_coords)*kernel_value[4];
    temp += texture2D(image_texture, texture_coords + vec2(x_offset, 0.0))*kernel_value[5];
    temp += texture2D(image_texture, texture_coords + vec2(-x_offset, -y_offset))*kernel_value[6];
    temp += texture2D(image_texture, texture_coords + vec2(0.0, -y_offset))*kernel_value[7];
    temp += texture2D(image_texture, texture_coords + vec2(x_offset, -y_offset))*kernel_value[8];

    // we multiply the sum of the neighbors' color by the scale factor to have a edge detection effect
    // and we add the original image to it
    out_Color = scale_factor * temp + original;
}
