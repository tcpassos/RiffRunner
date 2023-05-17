#include "menu.hpp"
#include "resource_manager.h"
#include "scene.h"
#include "sound.h"
#include "sprite.hpp"

const int FAILURE_RESTART = 0;
const int FAILURE_GIVEUP = 1;

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_failure(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
        
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }

    //Deal with selected options when ENTER is pressed
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        Sound::stopAll();
        if (menu->getItemIndex() == FAILURE_RESTART)
            accept(window, SceneGame);
        if (menu->getItemIndex() == FAILURE_GIVEUP)
            accept(window, SceneMenu);
    }
    //Return to menu
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        Sound::stopAll();
        accept(window, SceneMenu);
    }

}

// ======================================================================================
// Failure scene
// ======================================================================================
SceneId acceptFailure(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Failure menu
    Menu failureMenu(width, height);
    failureMenu.setPosition(width / 12 * 3, height / 8 * 6);

    failureMenu.addItem("<Reiniciar>");
    failureMenu.addItem("<Desistir>");

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_failure);
    glfwSetWindowUserPointer(window, &failureMenu);

    // Background
    Texture2D backgroundTexture = ResourceManager::loadTexture("assets/img/sadPepe.jpg", "Failure");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setSize(width, height);

    // Failure audios
    Sound failureSong("assets/sound/musicaTristeDoNaruto.wav");
    failureSong.play();
    Sound failureAudio("assets/sound/rocky.wav");
    failureAudio.setVolume(45);
    failureAudio.play();
    Sound naoConsegue("assets/sound/naoconsegue.wav");


    while (!glfwWindowShouldClose(window)) {
        int oldOption = failureMenu.getItemIndex();

        glViewport(0, 0, width, height);
        glfwPollEvents();

        if (failureMenu.getItemIndex() != oldOption && failureMenu.getItemIndex() == 1)
            naoConsegue.play();

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        failureMenu.draw();
        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}