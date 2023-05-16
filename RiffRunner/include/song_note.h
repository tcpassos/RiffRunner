#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <cmath>

#include "light_stripe.hpp"
#include "sprite.hpp"
#include "rect.h"
#include "resource_manager.h"

enum NoteState {
    NoteUnpressed,
    NoteHolding,
    NoteDisabled
};

class SongNote {
public:
    SongNote(Sprite &track, unsigned int value, unsigned int tailLength = 0);
    bool checkInput(unsigned int input);
    bool intersects(Rect rect);
    bool isBefore(Rect rect);
    bool isAfter(Rect rect);
    bool isConsumed();
    bool hasTail();
    unsigned int getIndex();
    unsigned int getValue();
    Rect getBounds();
    NoteState getState();
    void disable();
    unsigned int hold(unsigned int positionY);
    void move(float value);
    void draw(GLFWwindow* window, bool specialActive = false);

private:
    unsigned int value;
    unsigned int index;
    NoteState state;
    bool consumed;
    Sprite* note;
    LightStripe* tail;
    unsigned int tailLength;
    double lastHoldingPointTime;
    Rect* noteNormalTextureRect;
    Rect* noteSpecialTextureRect;
    glm::vec3 originalColor;
};