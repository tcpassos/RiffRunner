#include <iostream>

#include <string>
#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "resource_manager.h"
#include "sprite.h"
#include <vector>
#include <string.h>
#include "audio_player.h"
#include "text_renderer.h"
#include "menu.h"

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_difficulty_selector(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);
    int oldOption = menu->getItemIndex();

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }
    if (oldOption != menu->getItemIndex())
        switch (menu->getItemIndex()) {
            case 0:
                menu->playSound("resources/sound/izzi.wav");
                break;
            case 1:
                menu->playSound("resources/sound/family.wav");
                break;
            case 2:
                menu->playSound("resources/sound/serjao.wav");
                break;
            case 3:
                menu->playSound("resources/sound/chuck.wav");
                break;
        }

}

// ======================================================================================
// Difficulty selector scene
// ======================================================================================
SceneId acceptDifficultySelector(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Difficulty selector menu
    Menu difficultySelectorMenu(width, height, 0, 0, "resources/fonts/Queen of Clubs.otf");
    difficultySelectorMenu.setAlignmentHorizontal();

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_difficulty_selector);
    glfwSetWindowUserPointer(window, &difficultySelectorMenu);

    // Load album covers
    std::vector<Sprite> difficulties;
    for (int x = 0; x < 4; x++) {
        std::string file = to_string(x);
        Texture2D texture = ResourceManager::LoadTexture( ("resources/img/difficulty/" + file + ".jpg").c_str(), "dificulty_" + file);
        Sprite difficulty(texture);
        float difficultyWidth = width;
        float difficultyHeight = height / 4;
        difficulty.setPosition(0, difficultyHeight*x);
        difficulty.setSize(difficultyWidth, difficultyHeight);
        difficulties.push_back(difficulty);

        difficultySelectorMenu.addItem(file);

    }

    // Background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/difficulty/fundo.png", "difficultySelectorBackground");
    Sprite backgroundImage(backgroundTexture);

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            return SceneMusicSelector;

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        difficulties[difficultySelectorMenu.getItemIndex()].draw(window);
        difficultySelectorMenu.draw();

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}