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
    void disable();
    Rect getBounds();
    void move(int value);
    void draw(GLFWwindow* window);

private:
    unsigned int value;
    bool disabled;
    Sprite* note;
    RectangleShape* tail;
    unsigned int tailLength;
};