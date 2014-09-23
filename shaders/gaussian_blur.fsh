#version 330

// the size of the maximum implemented kernel
const int max_kernel_size = 81;

// the original image's texture
uniform sampler2D image_texture;

// the offset in x coord
uniform float x_offset;

// the offset in y coord
uniform float y_offset;

// the size of the actual kernel
uniform int kernel_size;

// the array containing all the kernel values
uniform float kernel_value[max_kernel_size];

// the texture's coords
in vec2 texture_coords;

// the pixel's out color rgba
out vec4 out_Color;

void main(void) {

    // temporary vec4 used to contain the sum of the neighbors' color
    vec4 temp = vec4(0.0);

    int x;
    int y;
    int i = 0;

    // loops going from the upper left corner to the bottom right corner
    for(y = kernel_size/2; y >= -kernel_size/2; y--) {
        for(x = -kernel_size/2; x <= kernel_size/2; x++) {

            // summing the value of the pixel's color located in (x, y) times the kernel value
            temp += texture2D(image_texture, texture_coords + vec2(x*x_offset, y*y_offset)) * kernel_value[i];
            i++;
        }
    }

    // assigning the sum of its neighbors' color to the pixel's color
    out_Color = temp;
}
