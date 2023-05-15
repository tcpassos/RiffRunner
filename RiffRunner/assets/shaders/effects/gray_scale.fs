#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texBuff;

void main() {
    vec4 color = texture(texBuff, texCoord);
    float gray = (color.r + color.g + color.b) / 3.0;
    FragColor = vec4(gray, gray, gray, color.a);
}
