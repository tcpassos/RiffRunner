#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "resource_manager.h"
#include "texture.h"
#include "sound.h"
#include "sprite.hpp"
#include "text_renderer.h"
#include "texture.h"

class HUD {
public:
    HUD();
    void incrementPerformance();
    void decrementPerformance();
    void addPoints(unsigned int points);
    void incrementStreak();
    void clearStreak();
    void activateSpecial();
    bool isSpecialActive() { return this->specialActive; }
    unsigned int getPerformance() { return this->performance; }
    unsigned int getScore() { return this->score; }
    unsigned int getMultiplier();
    void update(double elapsedTime);
    void draw(GLFWwindow* window);

private:
    unsigned int performance;
    unsigned int limit;
    unsigned int score;
    unsigned int streak;
    int specialCounter;
    float specialBarUnit;
    bool specialActive;
    double lastUpdateTime;

    Sprite* indicator;
    Sprite* pointer;
    Sprite* display;
    TextRenderer* scoreText;
    Sprite* multiplier;
    Sprite* specialBar;
    Sound* specialSound;
    EffectShine shineEffect;

    std::vector<Texture2D> multiplierTextures;

    void updatePerformanceIndicator();
    void updateMultiplier();
    void updateSpecialBar();
};