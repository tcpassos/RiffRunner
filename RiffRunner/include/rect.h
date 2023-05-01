#pragma once

#include <glm/glm.hpp>

class Rect {
public:
    glm::vec2 topLeft;
    glm::vec2 topRight;
    glm::vec2 bottomLeft;
    glm::vec2 bottomRight;

    Rect(glm::vec2 tl, glm::vec2 tr, glm::vec2 bl, glm::vec2 br) :
        topLeft(tl), topRight(tr), bottomLeft(bl), bottomRight(br) {}

    void setTopLeft(glm::vec2 tl) { topLeft = tl; }
    void setTopRight(glm::vec2 tr) { topRight = tr; }
    void setBottomLeft(glm::vec2 bl) { bottomLeft = bl; }
    void setBottomRight(glm::vec2 br) { bottomRight = br; }
};
