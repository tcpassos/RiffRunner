#version 330 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D texBuff;
uniform float time;
uniform float intensity;
uniform float speed;

void main() {
	float oscilation = 0.5 + 0.5 * sin(time * 2.0 * 3.14159 * speed);
	color = texture(texBuff, texCoord) * (1.0 + oscilation * intensity);
}