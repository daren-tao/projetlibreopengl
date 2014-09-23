#version 330

// the original image's texture
uniform sampler2D image_texture;

// the offset in x coord
uniform float x_offset;

// the offset in y coord
uniform float y_offset;

// the array containing all the kernel values
uniform float kernel_value[9];

// the texture's coords
in vec2 texture_coords;

// the pixel's out color rgba
out vec4 out_Color;

void main(void) {
    // temporary vec4 used to contain the sum of the neighbors' color
    vec4 sum = vec4(0.0);

    // summing the value of the pixel's color located in (x, y) times the kernel value
    vec4 temp = texture2D(image_texture, texture_coords + vec2(-x_offset, y_offset));
    sum += temp * kernel_value[0];
    temp = texture2D(image_texture, texture_coords + vec2(0.0, +y_offset));
    sum += temp * kernel_value[1];
    temp = texture2D(image_texture, texture_coords + vec2(x_offset, y_offset));
    sum += temp * kernel_value[2];
    temp = texture2D(image_texture, texture_coords + vec2(-x_offset, 0.0));
    sum += temp * kernel_value[3];
    temp = texture2D(image_texture, texture_coords);
    sum += temp * kernel_value[4];
    temp = texture2D(image_texture, texture_coords + vec2(x_offset, 0.0));
    sum += temp * kernel_value[5];
    temp = texture2D(image_texture, texture_coords + vec2(-x_offset, -y_offset));
    sum += temp * kernel_value[6];
    temp = texture2D(image_texture, texture_coords + vec2(0.0, -y_offset));
    sum += temp * kernel_value[7];
    temp = texture2D(image_texture, texture_coords + vec2(x_offset, -y_offset));
    sum += temp * kernel_value[8];
    out_Color = sum;

    // in order to avoid getting a peak in only one color,
    // we get the max of rgb to draw the discontinuity in grayscale
    out_Color.r = max(out_Color.r,out_Color.g);
    out_Color.r = max(out_Color.r,out_Color.b);
    out_Color = out_Color.rrra;
}
