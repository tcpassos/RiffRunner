#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform vec3 neonColor = vec3(1.0, 1.0, 1.0);
uniform float radius = 0.005;
uniform float intensity = 1.3;

const float margin = 0.1;

float getGlow(float distance, float radius, float intensity) {
    return pow(radius / distance, intensity);
}

void main() {
    float distanceX = texCoord.x - 0.5;
    float distanceY = 0;

    // Top
    if (texCoord.y < margin) {
        distanceY = texCoord.y - margin;
    // Bottom
    } else if (texCoord.y > (1.0 - margin)) {
        distanceY = texCoord.y - (1.0 - margin);
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
