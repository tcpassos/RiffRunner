#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "sprite.h"
#include "rect.h"
#include "rectangle_shape.h"
#include "resource_manager.h"

class SongNote {
public:
    SongNote(Sprite &track, unsigned int value, unsigned int tailLength = 0);
    bool checkInput(unsigned int input);
    bool intersects(Rect rect);
    bool isBefore(Rect rect);
    bool isAfter(Rect rect);
    bool isDisabled();
    bool isConsumed();
    bool hasTail();
    unsigned int getIndex();
    unsigned int getValue();
    Rect getBounds();
    void disable();
    unsigned int hold(unsigned int positionY);
    void move(float value);
    void draw(GLFWwindow* window, bool specialActive = false);

private:
    unsigned int value;
    unsigned int index;
    bool disabled;
    bool consumed;
    Sprite* note;
    RectangleShape* tail;
    unsigned int tailLength;
    double lastHoldingPointTime;
    Rect* noteNormalTextureRect;
    Rect* noteSpecialTextureRect;
};