#pragma once

#include "rect.h"

class Frame {
public:
	float duration;
	Rect* rect;

	Frame(float duration, float width, float height, unsigned int horizontalOffset = 0, unsigned int verticalOffset = 0) {
		this->duration = duration;
		float left = width * horizontalOffset;
		float top = height * verticalOffset;
		float right = left + width;
		float bottom = top  + height;
		this->rect = new Rect(left, top, right, bottom);
	}
};