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
    bool hasTail();
    unsigned int getIndex();
    Rect getBounds();
    void disable();
    void update(unsigned int positionY);
    void move(int value);
    void draw(GLFWwindow* window);

private:
    unsigned int value;
    unsigned int index;
    bool disabled;
    Sprite* note;
    RectangleShape* tail;
    unsigned int tailLength;
};