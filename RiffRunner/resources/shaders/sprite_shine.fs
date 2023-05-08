#version 460

in vec2 texCoord;
out vec4 color;

uniform sampler2D texBuff;
uniform vec4 texColor;

uniform float time;
uniform float effectIntensity;
uniform float effectSpeed;

void main() {
	float oscilation = 0.5 + 0.5 * sin(time * 2.0 * 3.14159 * effectSpeed);
	color = texture(texBuff, texCoord) * texColor * (1.0 + oscilation * effectIntensity);
}