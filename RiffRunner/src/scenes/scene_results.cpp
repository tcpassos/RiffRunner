#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "game_info.h"
#include "menu.hpp"
#include "resource_manager.h"
#include "scene.h"
#include "sprite.hpp"

// ======================================================================================
// Results scene
// ======================================================================================
SceneId acceptResults(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    TextRenderer textRenderer(width, height, Font("assets/fonts/Queen of Clubs.otf", 52));

    while (!glfwWindowShouldClose(window)) {

        //Deal with selected options when ENTER is pressed
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            return SceneMenu;
        }

        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        textRenderer.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0)); // white
        std::stringstream scoreString;
        scoreString << "Score: " << std::fixed << GameInfo::finalScore;
        textRenderer.renderText(scoreString.str(), width / 2, height / 2 - 70);
        std::stringstream progressString;
        progressString << "Progresso: " << std::fixed << std::setprecision(2) << ((float)GameInfo::hitNotes / GameInfo::noteCount) * 100 << "%";
        textRenderer.renderText(progressString.str(), width / 2, height / 2);

        textRenderer.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0)); // red
        textRenderer.renderText("[ Continue ]", width / 2, height / 2 + 80);

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}