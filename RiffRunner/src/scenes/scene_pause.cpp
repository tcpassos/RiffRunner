#include "menu.h"
#include "resource_manager.h"
#include "scene.h"
#include "sound.h"
#include "sprite.h"

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_pause(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }
}

// ======================================================================================
// Pause scene
// ======================================================================================
SceneId acceptPause(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Background
    Texture2D backgroundTexture = ResourceManager::loadTexture("resources/img/pauseMenu.png", "Pause");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setOrigin(glm::vec2(backgroundTexture.width / 2, backgroundTexture.height / 2));
    backgroundImage.setPosition(glm::vec2(width / 2, height / 2));

    // Pause menu
    Menu pauseMenu(width, height, 0, 0);

    const int PAUSE_CONTINUE = pauseMenu.addItem("<Continuar>");;
    const int PAUSE_RESTART = pauseMenu.addItem("<Reiniciar>");;
    const int PAUSE_EXIT = pauseMenu.addItem("<Desistir>");;

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_pause);
    glfwSetWindowUserPointer(window, &pauseMenu);

    while (!glfwWindowShouldClose(window)) {

        //Deal with selected options when ENTER is pressed
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (pauseMenu.getItemIndex() == PAUSE_CONTINUE) {
                return SceneCurrent;
            }
            if (pauseMenu.getItemIndex() == PAUSE_RESTART) {
                return SceneGame;
            }
            if (pauseMenu.getItemIndex() == PAUSE_EXIT) {
                return SceneMenu;
            }
        }

        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        pauseMenu.draw();
        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}