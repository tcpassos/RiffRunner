#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform vec3 neonColor;
uniform float radius;
uniform float intensity;
uniform float size;
uniform vec2 resolution;
uniform vec2 position;

float getGlow(float distance, float radius, float intensity) {
    return pow(radius / distance, intensity);
}

void main() {
    vec2 normalizedPosition = position / resolution;
    float normalizedSize = size / resolution.y;
    float marginTop = normalizedPosition.y - normalizedSize;
    float marginBottom = normalizedPosition.y;
    float distanceX = texCoord.x - normalizedPosition.x;
    float distanceY = 0;

    // Top
    if (texCoord.y < marginTop) {
        distanceY = texCoord.y - marginTop;
    // Bottom
    } else if (texCoord.y > marginBottom) {
        distanceY = texCoord.y - marginBottom;
    }

    float distance = length(vec2(distanceX, distanceY));
    float strength = getGlow(distance, radius, intensity);

    vec3 col = neonColor;
    // Tone mapping
    col = 1.0 - exp(-col);
    // Gamma correction
    col = pow(col, vec3(0.4545));
    // Output to the screen
    fragColor = vec4(col, strength);
}
