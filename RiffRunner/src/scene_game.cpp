#include <iostream>

#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "resource_manager.h"
#include "sprite.h"
#include "animation.h"
#include "hud.h"

// ======================================================================================
// Game scene
// ======================================================================================
SceneId acceptGame(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Game background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/game_background.jpg", "gameBackground");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setSize(glm::vec3(windowWidth, windowHeight, 1.0f));

    // Performance indicator | Score | Special multiplier
    HUD hud;

    // Track
    Texture2D trackTexture = ResourceManager::LoadTexture("resources/img/track.jpg", "track");
    Sprite track(trackTexture);
    track.setOrigin(track.getSize() / 2.0f);
    track.setPosition(windowWidth / 2, windowHeight / 2);
    track.getProjection()->setRotation(glm::vec3(glm::radians(-50.0f), 0.0f, 0.0f));
    track.getProjection()->moveY(-100.0f);
    track.getProjection()->moveZ(-55.0f);

    // Flames
    Texture2D flamesTexture = ResourceManager::LoadTexture("resources/img/flames.png", "flames");
    Sprite flames(flamesTexture);
    flames.setSize(40.0f, 50.0f);
    Animation flamesAnimation(flames);
    for (int flameOffset = 0; flameOffset < 8; flameOffset++) {
        flamesAnimation.addFrame(*(new Frame(35.0f, flames.getSize().x / 8, flames.getSize().y, flameOffset)));
    }

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, windowWidth, windowHeight);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Score
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            hud.decrementPerformance();
            hud.clearStreak();
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            hud.incrementPerformance();
            hud.incrementStreak();
            hud.addPoints(1);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            flamesAnimation.reset();
        }
        flamesAnimation.update();

        if (hud.getPerformance() == 0)
            return SceneFailure;

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ==========================================================

        // Background
        backgroundImage.draw(window);
        
        // HUD
        hud.draw(window);

        // Track
        Rect trackTextureRect = track.getTextureRect();
        trackTextureRect.top--;
        trackTextureRect.height--;
        track.setTextureRect(trackTextureRect);        
        track.draw(window);

        // Flames
        if (flamesAnimation.isRunning()) {
            flames.draw(window);
        }

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}