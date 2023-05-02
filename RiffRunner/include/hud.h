#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resource_manager.h"
#include "texture.h"
#include "sprite.h"
#include "text_renderer.h"

class HUD {
public:
    HUD();
    void incrementPerformance();
    void decrementPerformance();
    void addPoints(unsigned int points) { this->score += points; }
    void incrementStreak();
    void clearStreak();
    unsigned int getPerformance() { return this->performance; }
    unsigned int getScore() { return this->score; }
    void draw(GLFWwindow* window);

private:
    unsigned int performance;
    unsigned int limit;
    unsigned int score;
    unsigned int streak;
    float specialBarHeight;

    Sprite* indicator;
    Sprite* pointer;
    Sprite* display;
    TextRenderer* scoreText;
    Sprite* multiplier;
    Sprite* specialBar;

    void updatePerformanceIndicator();
    void updateMultiplier();
};