#pragma once

#include <glm/glm.hpp>

class Rect {
public:
    float left;
    float top;
    float width;
    float height;

    Rect(float l, float t, float w, float h) :
        left(l), top(t), width(w), height(h) {}

    Rect(glm::vec2 position, glm::vec2 size) :
        left(position.x), top(position.y), width(size.x), height(size.y) {}
};
