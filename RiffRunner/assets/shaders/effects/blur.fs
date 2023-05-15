#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texBuff;
uniform vec2 blur_size = vec2(.5, .5);
uniform vec4 v_colour = vec4(1.0, 1.0, 1.0, 1.0);;

void main() {
    ivec2 size = textureSize(texBuff, 0);

    float uv_x = texCoord.x * size.x;
    float uv_y = texCoord.y * size.y;

    vec4 sum = vec4(0.0);
    for (int n = 0; n < 9; ++n) {
        uv_y = (texCoord.y * size.y) + (blur_size.y * float(n - 4.5));
        vec4 h_sum = vec4(0.0);
        h_sum += texelFetch(texBuff, ivec2(uv_x - (4.0 * blur_size.x), uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x - (3.0 * blur_size.x), uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x - (2.0 * blur_size.x), uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x - blur_size.x, uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x, uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x + blur_size.x, uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x + (2.0 * blur_size.x), uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x + (3.0 * blur_size.x), uv_y), 0);
        h_sum += texelFetch(texBuff, ivec2(uv_x + (4.0 * blur_size.x), uv_y), 0);
        sum += h_sum / 9.0;
    }

    FragColor = sum / 9.0;
}