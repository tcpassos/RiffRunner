#version 460

out vec4 color;

uniform vec4 fillColor;

uniform float time;
uniform float effectIntensity;
uniform float effectSpeed;

void main() {
	float oscilation = 0.5 + 0.5 * sin(time * 2.0 * 3.14159 * effectSpeed);
	color = fillColor * (1.0 + oscilation * effectIntensity);
}