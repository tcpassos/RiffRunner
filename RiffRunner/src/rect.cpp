#include "rect.h"

bool Rect::intersects(Rect rect) const {
    return this->intersectsX(rect) && this->intersectsY(rect);
}

bool Rect::intersectsX(Rect rect) const {
    return (this->left >= rect.left && this->left < rect.width) ||
           (rect.left >= this->left && rect.left < this->width);
}

bool Rect::intersectsY(Rect rect) const {
    return (this->top >= rect.top && this->top < rect.height) ||
           (rect.top >= this->top && rect.top < this->height);
}