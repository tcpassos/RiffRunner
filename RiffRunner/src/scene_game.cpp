#include <iostream>

#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "resource_manager.h"
#include "sprite.h"
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

    // Performance indicator
    HUD hud;

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, windowWidth, windowHeight);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            hud.decrementPerformance();
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            hud.incrementPerformance();
            hud.addPoints(1);
        }

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);
        // ==========================================================

        backgroundImage.draw(window);
        hud.draw(window);

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}