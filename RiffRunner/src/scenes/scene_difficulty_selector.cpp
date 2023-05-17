#include <vector>
#include <string>

#include "menu.hpp"
#include "resource_manager.h"
#include "scene.h"
#include "sprite.hpp"
#include "game_info.h"

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_difficulty_selector(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);
    int oldOption = menu->getItemIndex();

    //Select the difficulty with UP and DOWN
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }

    //Starts to play the game with enter
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        GameInfo::selectedDifficulty = menu->getItemIndex();
        accept(window, SceneGame);
    }
    //Return to music selector with ESC
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        accept(window, SceneMusicSelector);
    }

    if (oldOption != menu->getItemIndex())
        switch (menu->getItemIndex()) {
            case 0:
                menu->playSound("assets/sound/izzi.wav");
                break;
            case 1:
                menu->playSound("assets/sound/family.mp3");
                break;
            case 2:
                menu->playSound("assets/sound/serjao.wav");
                break;
            case 3:
                menu->playSound("assets/sound/chuck.wav");
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
    Menu difficultySelectorMenu(width, height);

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_difficulty_selector);
    glfwSetWindowUserPointer(window, &difficultySelectorMenu);

    // Load album covers
    std::vector<Sprite> difficulties;
    for (int x = 0; x < 4; x++) {
        string file = to_string(x);
        Texture2D texture = ResourceManager::loadTexture( ("assets/img/difficulty/" + file + ".jpg").c_str(), "dificulty_" + file);
        Sprite difficulty(texture);
        float difficultyWidth = width;
        float difficultyHeight = height / 4;
        difficulty.setPosition(0, difficultyHeight*x);
        difficulty.setSize(difficultyWidth, difficultyHeight);
        difficulties.push_back(difficulty);

        difficultySelectorMenu.addItem(file);

    }

    // Background
    Texture2D backgroundTexture = ResourceManager::loadTexture("assets/img/difficulty/fundo.png", "difficultySelectorBackground");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setSize(width, height);

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glfwPollEvents();      

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        difficulties[difficultySelectorMenu.getItemIndex()].draw(window);

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}