#include "rect.h"

bool Rect::intersects(Rect rect) const {
    return this->intersectsX(rect) && this->intersectsY(rect);
}

bool Rect::intersectsX(Rect rect) const {
    float aLeft = this->left;
    float aRight = this->left + this->width;
    float bLeft = rect.left;
    float bRight = rect.left + rect.width;
    return (aLeft >= bLeft && aLeft < bRight) || (bLeft >= aLeft && bLeft < aRight);
}

bool Rect::intersectsY(Rect rect) const {
    float aTop = this->top;
    float aBottom = this->top + this->height;
    float bTop = rect.top;
    float bBottom = rect.top + rect.height;
    return (aTop >= bTop && aTop < bBottom) || (bTop >= aTop && bTop < aBottom);
}